// -------------------------------------------------------
// 名前:	CollisionManager
// 内容:	衝突判定を管理するクラス
//			オブジェクトがCollision生成時に
//			このクラスに登録され、
//			当たり判定の更新を行う
// 作成:	池田桜輔
// -------------------------------------------------------

// インクルード
#include "pch.h"
#include "CollisionManager.h"
#include "Interface/IObject.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Game/Messenger/EventMessenger.h"
#include "Libraries/Mylib/DebugDraw.h"
#include "Libraries/Mylib/DebugString.h"
#include <future>

/// <summary>
/// コンストラクタ
/// </summary>
CollisionManager::CollisionManager()
	: m_basicEffect(nullptr)
	, m_inputLayout(nullptr)
	, m_primitiveBatch(nullptr)
	, m_obbs()
	, m_spheres()
	, m_obbProxies()
	, m_drawFlag(false)
	, m_isProcessingCollisions(false)
	, m_exitRequested(false)
	, m_updateRequested(false)
{
	// 生成と同時に初期化を行う
	Initialize();
	// 別スレッドに衝突処理を登録
	RegisterThread();
}

/// <summary>
/// 別スレッドに衝突処理を登録
/// </summary>
inline void CollisionManager::RegisterThread()
{
	// スレッド開始前に終了フラグを初期化
	m_exitRequested = false;
	m_updateRequested = false;

	// 別スレッドで実行を回し続ける
	m_collisionThread = std::thread([this]()
		{
			while (true)
			{
				// ミューテックスロックと条件変数待機
				std::unique_lock<std::mutex> lock(m_mutex);
				m_cv.wait(lock, [this]() { return m_updateRequested || m_exitRequested; });

				// 終了が指示された場合はループを抜ける
				if (m_exitRequested)
				{
					break;
				}

				// 衝突判定処理を実行（各種コライダ間）
				// ロックを保持したまま処理を実行
				CheckCollisionOBBToSphere();
				CheckCollisionSphereToSphere();

				// 処理完了後にフラグをリセット
				m_updateRequested = false;

				// 処理完了を通知
				lock.unlock();
				m_cv_complete.notify_one();
			}
			// スレッド終了時のクリーンアップ処理
			Clear();
		});
}

/// <summary>
/// デストラクタ
/// </summary>
CollisionManager::~CollisionManager()
{
	// 終了リクエストをセット
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_exitRequested = true;
	}

	// 条件変数を通知して待機中のスレッドを起こす
	m_cv.notify_all();

	// 別スレッド終了
	ExitThread();

	// リセット
	Clear();
}

/// <summary>
/// 別スレッドの終了処理
/// </summary>
inline void CollisionManager::ExitThread()
{
	// まず終了リクエストをセット
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_exitRequested = true;
	}

	// 条件変数を通知して待機中のスレッドを起こす
	m_cv.notify_all();

	// スレッドが生きていればjoinする（終了まで待機する）
	if (m_collisionThread.joinable())
	{
		try
		{
			m_collisionThread.join();
		}
		catch (const std::system_error& e)
		{
			// スレッドjoin失敗のログを出力
			std::string errorMsg = "Thread join failed: " + std::string(e.what());
			MessageBoxA(nullptr, errorMsg.c_str(), "エラー", MB_OK | MB_ICONERROR);
		}
	}
}

/// <summary>
/// 初期化処理
/// </summary>
void CollisionManager::Initialize()
{
	CommonResources* resources = CommonResources::GetInstance();

	auto device = resources->GetDeviceResources()->GetD3DDevice();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();

	// ベーシックエフェクトを作成する
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);

	// 入力レイアウトを作成する
	DX::ThrowIfFailed(
		DirectX::CreateInputLayoutFromEffect<DirectX::VertexPositionColor>(
			device,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf())
	);

	// プリミティブバッチを生成
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(context);

	// キーボードを作成する
	m_keyboardState = DirectX::Keyboard::Get().GetState();

	// イベントの登録
	AddEventMessenger();
}

/// <summary>
/// イベントの登録
/// </summary>
void CollisionManager::AddEventMessenger()
{
	// OBBの登録イベント
	EventMessenger::Attach(EventList::AddOBBCollision, std::bind(&CollisionManager::AddCollision<DirectX::BoundingOrientedBox>, this, std::placeholders::_1));
	// Sphereの登録イベント
	EventMessenger::Attach(EventList::AddSphereCollision, std::bind(&CollisionManager::AddCollision<DirectX::BoundingSphere>, this, std::placeholders::_1));
	// 衝突判定の削除イベント - キュー登録に変更
	EventMessenger::Attach(EventList::DeleteCollision, std::bind(&CollisionManager::QueueDeleteCollision, this, std::placeholders::_1));
}

/// <summary>
/// 球と球の当たり判定
/// </summary>
inline void CollisionManager::CheckCollisionSphereToSphere()
{
	if (m_exitRequested) return;

	// 球同士による当たり判定(同じ球同士で衝突しないように)
	const size_t sphereCount = m_spheres.size();
	for (size_t i = 0; i < sphereCount; i++)
	{
		// nullチェック
		if (m_spheres[i].object == nullptr) continue;
		if (m_spheres[i].collision == nullptr) continue;

		for (size_t j = i + 1; j < sphereCount; j++)
		{
			// nullチェック
			if (m_spheres[j].object == nullptr) continue;
			if (m_spheres[j].collision == nullptr) continue;

			// 球同士の当たり判定
			if (m_spheres[i].collision->Intersects(*m_spheres[j].collision))
			{
				// ゴブリン同士と衝突している場合は処理を行わない
				if (IsGoblinCollision(m_spheres[i], m_spheres[j])) continue;

				// 衝突したときに相手に渡すデータを作成
				InterSectData sphereData1 = { m_spheres[i].objType, m_spheres[i].colType, m_spheres[i].collision };
				InterSectData sphereData2 = { m_spheres[j].objType, m_spheres[j].colType, m_spheres[j].collision };

				// 最終的な安全チェック
				if (m_spheres[i].object != nullptr) m_spheres[i].object->HitAction(sphereData2);
				if (m_spheres[j].object != nullptr) m_spheres[j].object->HitAction(sphereData1);
			}
		}
	}
}

/// <summary>
/// OBBと球の当たり判定
/// </summary>
inline void CollisionManager::CheckCollisionOBBToSphere()
{
	if (m_exitRequested) return;

	// サイズを事前に取得（変更される可能性を防ぐ）
	const size_t obbCount = m_obbs.size();
	const size_t sphereCount = m_spheres.size();

	// プロキシ球のサイズを調整
	while (m_obbProxies.size() < obbCount) {
		m_obbProxies.push_back(nullptr);
	}

	// OBBのプロキシと球の当たり判定
	for (size_t i = 0; i < obbCount; i++)
	{
		// nullチェック
		if (m_obbs[i].object == nullptr) continue;
		if (m_obbs[i].collision == nullptr) continue;

		// OBBのプロキシ球の中心をOBBの中心に設定
		try {
			m_obbProxies[i] = CreateProxySphere(m_obbs[i].collision);
		}
		catch (const std::exception& e) {
			// プロキシ球作成失敗時はこのOBBをスキップ
			continue;
		}

		for (size_t j = 0; j < sphereCount; j++)
		{
			// nullチェック
			if (m_spheres[j].object == nullptr) continue;
			if (m_spheres[j].collision == nullptr) continue;
			if (m_obbProxies[i] == nullptr) continue;

			// プロキシと衝突していなければ次の球に移る
			if (!m_obbProxies[i]->Intersects(*m_spheres[j].collision))	continue;

			// OBBと球の当たり判定
			if (m_obbs[i].collision->Intersects(*m_spheres[j].collision))
			{
				// 衝突したときに相手に渡すデータを作成
				InterSectData obbData = { m_obbs[i].objType, m_obbs[i].colType,	m_obbProxies[i].get() };
				InterSectData sphereData = { m_spheres[j].objType, m_spheres[j].colType, m_spheres[j].collision };

				// 最終的な安全チェック
				if (m_obbs[i].object != nullptr) m_obbs[i].object->HitAction(sphereData);
				if (m_spheres[j].object != nullptr) m_spheres[j].object->HitAction(obbData);
			}
		}
	}
}

/// <summary>
/// ゴブリン同士と衝突しているかを検知
/// </summary>
/// <param name="collisionA">衝突判定A</param>
/// <param name="collisionB">衝突判定B</param>
/// <returns>ゴブリン同士と当たっているか</returns>
bool CollisionManager::IsGoblinCollision(
	const CollisionData<DirectX::BoundingSphere>& collisionA,
	const CollisionData<DirectX::BoundingSphere>& collisionB
)
{
	// ゴブリン同士と衝突を行っているか
	if (collisionA.objType == ObjectType::Goblin && collisionB.objType == ObjectType::Goblin)
	{
		if (collisionB.object != nullptr) {
			// 衝突したときに相手に渡すデータを作成
			InterSectData goblinData = { collisionA.objType, collisionA.colType, collisionA.collision };
			// 衝突したときの処理を呼び出す
			collisionB.object->HitAction(goblinData);
		}
		return true;
	}

	return false;
}

/// <summary>
/// 削除要求をキューに登録する
/// </summary>
/// <param name="args">DeleteCollisionData型</param>
void CollisionManager::QueueDeleteCollision(void* args)
{
	// argsはDeleteCollisionData構造体へのポインタ
	auto* deleteData = static_cast<DeleteCollisionData*>(args);

	// 不正な引数の場合は終了
	if (!deleteData) return;

	// 削除予定データをコピー
	DeleteCollisionData pendingDelete = *deleteData;

	// キューに追加（ロック付き）
	{
		std::lock_guard<std::mutex> lock(m_deleteQueueMutex);
		m_pendingDeleteQueue.push_back(pendingDelete);
	}
}

/// <summary>
/// 削除キューを処理する（内部メソッド）
/// </summary>
void CollisionManager::ProcessDeleteQueue()
{
	// 削除キューが空ならスキップ
	if (m_pendingDeleteQueue.empty()) return;

	// キューをロックして処理
	std::vector<DeleteCollisionData> localQueue;
	{
		std::lock_guard<std::mutex> lock(m_deleteQueueMutex);
		// キューの内容をローカルにコピーしてからクリア
		localQueue = std::move(m_pendingDeleteQueue);
		m_pendingDeleteQueue.clear();
	}

	// ロックを解放した状態で実際の削除処理を行う
	for (const auto& deleteData : localQueue)
	{
		// 削除対象のオブジェクトとコリジョンタイプを取得
		IObject* targetObject = deleteData.object;
		CollisionType collType = deleteData.collType;

		if (!targetObject) continue;

		// コリジョンタイプに応じた削除処理
		if (collType == CollisionType::OBB)
		{
			// OBB要素数を取得
			size_t before = m_obbs.size();

			// OBBコンテナから削除
			m_obbs.erase(std::remove_if(m_obbs.begin(), m_obbs.end(),
				[targetObject](const auto& collision) {
					return collision.object == targetObject;
				}),
				m_obbs.end());

			// オブジェクトが削除された場合、対応するプロキシも削除
			if (before != m_obbs.size() && !m_obbProxies.empty()) {
				// プロキシ配列のサイズをOBB配列に合わせる
				m_obbProxies.resize(m_obbs.size());
			}
		}
		else if (collType == CollisionType::Sphere)
		{
			// Sphereコンテナから削除
			m_spheres.erase(std::remove_if(m_spheres.begin(), m_spheres.end(),
				[targetObject](const auto& collision) {
					return collision.object == targetObject;
				}),
				m_spheres.end());
		}
	}
}

/// <summary>
/// 更新処理
/// ここで衝突の検知を行う
/// </summary>
void CollisionManager::Update()
{
	// 衝突処理中フラグを設定
	m_isProcessingCollisions = true;

	{
		// 衝突判定処理の実行をスレッドに通知
		std::lock_guard<std::mutex> lock(m_mutex);
		m_updateRequested = true;
	}

	// スレッドを起床させる
	m_cv.notify_one();

	// 衝突処理の完了を待機する
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		// updateRequestedがfalseになるまで待機（衝突処理完了のシグナル）
		m_cv_complete.wait(lock, [this]() { return !m_updateRequested || m_exitRequested; });
	}

	// 衝突処理が完了したので、削除キューを処理
	ProcessDeleteQueue();

	// 衝突処理中フラグを解除
	m_isProcessingCollisions = false;

	// デバッグのみで実行可能
#ifdef _DEBUG
	// キーボードの入力を取得する
	m_keyboardState = DirectX::Keyboard::Get().GetState();
	m_keyboardStateTracker.Update(m_keyboardState);

	// F5キーが押されたら当たり判定の描画を切り替える
	if (m_keyboardStateTracker.IsKeyPressed(DirectX::Keyboard::F5)) {
		m_drawFlag = !m_drawFlag;
	}
#endif // !_DEBUG
}

/// <summary>
/// 描画処理
/// デバッグ中の当たり判定の描画を行う
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="projection">プロジェクション行列</param>
void CollisionManager::Render
(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
)
{
	if (!m_drawFlag) return;

	// 衝突判定の描画
	DrawCollision(view, projection);
}

/// <summary>
/// 登録された情報をクリアする
/// </summary>
void CollisionManager::Clear()
{
	// ロックを確保してからクリア
	std::lock_guard<std::mutex> lock(m_mutex);

	m_obbs.clear();
	m_spheres.clear();
	m_obbProxies.clear();
	m_pendingDeleteQueue.clear();
}

/// <summary>
/// 衝突判定を登録する
/// </summary>
/// <typeparam name="T">衝突判定の形状　球：有向境界ボックス</typeparam>
/// <param name="args">CollisionData型</param>
template<typename T>
void CollisionManager::AddCollision(void* args)
{
	// 引数を期待する型にキャスト
	auto* collisionData = static_cast<CollisionData<T>*>(args);

	if (!collisionData) return; // 不正な引数の場合は終了
	if (collisionData->object == nullptr) return; // オブジェクトがnullの場合は登録しない
	if (collisionData->collision == nullptr) return; // 衝突判定がnullの場合は登録しない

	// 衝突処理中は待機する
	while (m_isProcessingCollisions) {
		// ビジーウェイトを避けるため短いスリープ
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	// ロックを取得して登録
	std::lock_guard<std::mutex> lock(m_mutex);

	// 衝突判定データを対応するコンテナに追加
	if constexpr (std::is_same_v<T, DirectX::BoundingOrientedBox>)
	{
		// OBBを保存
		m_obbs.push_back(*collisionData);
		// OBBのプロキシ球を生成
		m_obbProxies.push_back(CreateProxySphere(collisionData->collision));
	}
	else if constexpr (std::is_same_v<T, DirectX::BoundingSphere>)
	{
		// Sphereを保存
		m_spheres.push_back(*collisionData);
	}
}

/// <summary>
/// 有向境界ボックスの処理を軽くするためのプロキシ球を作成
/// </summary>
/// <param name="collision">有向境界ボックス</param>
/// <returns>プロキシ球</returns>
inline std::unique_ptr<DirectX::BoundingSphere> CollisionManager::CreateProxySphere(const DirectX::BoundingOrientedBox* collision)
{
	// nullチェック
	if (collision == nullptr) {
		return nullptr;
	}

	// クォータニオンが有効かチェック
	DirectX::SimpleMath::Quaternion orientation = collision->Orientation;
	if (abs(orientation.LengthSquared() - 1.0f) > 0.01f)
	{
		// 正規化されていないクォータニオンの場合は修正
		DirectX::SimpleMath::Quaternion normalized = orientation;
		normalized.Normalize();

		// 新しいOBBを作成（constのため直接変更できないので）
		DirectX::BoundingOrientedBox fixedOBB;
		fixedOBB.Center = collision->Center;
		fixedOBB.Extents = collision->Extents;
		fixedOBB.Orientation = normalized;

		// 修正したOBBからプロキシ球を作成
		float radius = sqrtf(
			fixedOBB.Extents.x * fixedOBB.Extents.x +
			fixedOBB.Extents.y * fixedOBB.Extents.y +
			fixedOBB.Extents.z * fixedOBB.Extents.z
		);

		return std::make_unique<DirectX::BoundingSphere>(fixedOBB.Center, radius);
	}

	// BoundingSphereをOBBの中心と最大半径で作成
	float radius = sqrtf(
		collision->Extents.x * collision->Extents.x +
		collision->Extents.y * collision->Extents.y +
		collision->Extents.z * collision->Extents.z
	);

	return std::make_unique<DirectX::BoundingSphere>(collision->Center, radius);
}

/// <summary>
/// 衝突判定の描画を行う
/// デバッグ中のみ有効
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="projection">プロジェクション行列</param>
inline void CollisionManager::DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = CommonResources::GetInstance()->GetCommonStates();

	// 描画設定を行う
	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthDefault(), 0);
	context->RSSetState(states->CullNone());
	context->IASetInputLayout(m_inputLayout.Get());
	// ビュー行列と射影行列を設定
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(projection);
	m_basicEffect->Apply(context);
	// 描画開始
	m_primitiveBatch->Begin();

	// OBBの描画
	for (const auto& obb : m_obbs) {
		if (obb.collision)
			DX::Draw(m_primitiveBatch.get(), *obb.collision, DirectX::Colors::Red);
	}

	// Sphereの描画
	for (const auto& sphere : m_spheres) {
		if (sphere.collision)
			DX::Draw(m_primitiveBatch.get(), *sphere.collision, DirectX::Colors::Blue);
	}

	// OBBのプロキシ球の描画
	for (auto& sphere : m_obbProxies) {
		if (sphere)
			DX::Draw(m_primitiveBatch.get(), *sphere, DirectX::Colors::LimeGreen);
	}
	// 描画終了
	m_primitiveBatch->End();
}
