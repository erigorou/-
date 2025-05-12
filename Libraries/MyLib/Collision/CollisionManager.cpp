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
	, m_drawFlag(false)
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
		}
		// スレッド終了時のクリーンアップ処理（必要に応じて）
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
			// スレッドjoin失敗のログを出力（実際の環境に合わせて）
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
	// 衝突判定の削除イベント
	EventMessenger::Attach(EventList::DeleteCollision, std::bind(&CollisionManager::DeleteCollision, this, std::placeholders::_1));
}

/// <summary>
/// 球と球の当たり判定
/// </summary>
// 球と球の当たり判定
inline void CollisionManager::CheckCollisionSphereToSphere()
{
	if (m_exitRequested) return;

	// 球同士による当たり判定(同じ球同士で衝突しないように)
	for (int i = 0; i < static_cast<int>(m_spheres.size() - 1); i++)
	{
		// スレッド処理中に要素が削除された可能性をチェック
		if (i >= static_cast<int>(m_spheres.size())) break;
		if (m_spheres[i].object == nullptr) continue;

		for (int j = i + 1; j < static_cast<int>(m_spheres.size()); j++)
		{
			// スレッド処理中に要素が削除された可能性をチェック
			if (j >= static_cast<int>(m_spheres.size())) break;
			if (m_spheres[j].object == nullptr) continue;

			// 球同士の当たり判定
			if (m_spheres[i].collision->Intersects(*m_spheres[j].collision))
			{
				// ゴブリン同士の衝突時は処理を行わない
				if (IsGoblinCollision(m_spheres[i], m_spheres[j])) continue;

				// 衝突したときに相手に渡すデータを作成
				InterSectData sphereData1 = { m_spheres[i].objType, m_spheres[i].colType, m_spheres[i].collision };
				InterSectData sphereData2 = { m_spheres[j].objType, m_spheres[j].colType, m_spheres[j].collision };

				// 再度nullチェックを行う
				if (m_spheres[i].object != nullptr)
				{
					m_spheres[i].object->HitAction(sphereData2);
				}

				if (m_spheres[j].object != nullptr)
				{
					m_spheres[j].object->HitAction(sphereData1);
				}
			}
		}
	}
}

/// <summary>
/// OBBと球の当たり判定
/// </summary>
// OBBと球の当たり判定
inline void CollisionManager::CheckCollisionOBBToSphere()
{
	if (m_exitRequested) return;

	// OBBのプロキシと球の当たり判定
	for (int i = 0; i < static_cast<int>(m_obbs.size()); i++)
	{
		// スレッド処理中に要素が削除された可能性をチェック
		if (i >= static_cast<int>(m_obbs.size())) break;
		if (m_obbs[i].object == nullptr) continue;

		// OBBのプロキシ球の中心をOBBの中心に設定
		m_obbProxies[i] = CreateProxySphere(m_obbs[i].collision);

		for (int j = 0; j < static_cast<int>(m_spheres.size()); j++)
		{
			// スレッド処理中に要素が削除された可能性をチェック
			if (j >= static_cast<int>(m_spheres.size())) break;
			if (m_spheres[j].object == nullptr) continue;

			// プロキシと衝突していなければ次の球に移る
			if (!m_obbProxies[i]->Intersects(*m_spheres[j].collision)) continue;

			// OBBと球の当たり判定
			if (m_obbs[i].collision->Intersects(*m_spheres[j].collision))
			{
				// 衝突したときに相手に渡すデータを作成
				InterSectData obbData = { m_obbs[i].objType, m_obbs[i].colType, m_obbProxies[i].get() };
				InterSectData sphereData = { m_spheres[j].objType, m_spheres[i].colType, m_spheres[j].collision };

				// 再度nullチェックを行う
				if (m_obbs[i].object != nullptr)
				{
					m_obbs[i].object->HitAction(sphereData);
				}

				if (m_spheres[j].object != nullptr)
				{
					m_spheres[j].object->HitAction(obbData);
				}
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
	CollisionData<DirectX::BoundingSphere> collisionA,
	CollisionData<DirectX::BoundingSphere> collisionB
)
{
	// ゴブリン同士と衝突を行っているか
	if (collisionA.objType == ObjectType::Goblin && collisionB.objType == ObjectType::Goblin)
	{
		// 衝突したときに相手に渡すデータを作成
		InterSectData goblinData = { collisionA.objType, collisionA.colType, collisionA.collision };
		// 衝突したときの処理を呼び出す
		collisionB.object->HitAction(goblinData);
		return true;
	}

	return false;
}


/// <summary>
/// 更新処理
/// ここで衝突の検知を行う
/// </summary>
void CollisionManager::Update()
{
	{
		// 衝突判定処理の実行をスレッドに通知
		std::lock_guard<std::mutex> lock(m_mutex);
		m_updateRequested = true;
	}
	
	// スレッドを起床させる
	m_cv.notify_one();


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
	m_obbs.clear();
	m_spheres.clear();
	m_obbProxies.clear();
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

	// 衝突判定データを対応するコンテナに追加
	if constexpr (std::is_same_v<T, DirectX::BoundingOrientedBox>)
	{
		// OBBを保存
		m_obbs.push_back(*collisionData);
		// OBBのプロキシ球を生成
		m_obbProxies.push_back(CreateProxySphere(static_cast<const DirectX::BoundingOrientedBox*>(collisionData->collision)));
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
	// BoundingSphereをOBBの中心と最大半径で作成
	float radius = sqrtf(
		collision->Extents.x * collision->Extents.x +
		collision->Extents.y * collision->Extents.y +
		collision->Extents.z * collision->Extents.z
	);

	std::unique_ptr<DirectX::BoundingSphere> proxy = std::make_unique<DirectX::BoundingSphere>(collision->Center, radius);

	return std::move(proxy);
}

/// <summary>
/// 登録された衝突判定を解除する
/// </summary>
/// <param name="args">DeleteCollisionData型</param>
void CollisionManager::DeleteCollision(void* args)
{
	// Mutexロックを取得してスレッド間の同期を確保
	std::lock_guard<std::mutex> lock(m_mutex);

	// argsはDeleteCollisionData構造体へのポインタ
	auto* deleteData = static_cast<DeleteCollisionData*>(args);

	// 不正な引数の場合は終了
	if (!deleteData) return;

	// オブジェクトのポインタ
	IObject* targetObject = deleteData->object;

	// オブジェクトが有効かチェック
	if (targetObject == nullptr) return;

	// OBBの場合はOBBとプロキシ球の両方を削除
	if (deleteData->collType == CollisionType::OBB)
	{
		// 削除前にnullptrに置き換える（安全のため）
		for (auto& obb : m_obbs)
		{
			if (obb.object == targetObject)
			{
				obb.object = nullptr;
			}
		}

		// 対応するオブジェクトを削除
		m_obbs.erase(std::remove_if(m_obbs.begin(), m_obbs.end(),
			[targetObject](const auto& collision) {
				return collision.object == targetObject || collision.object == nullptr;
			}),
			m_obbs.end());
	}

	// Sphereの場合はSphereのみ削除
	else if (deleteData->collType == CollisionType::Sphere)
	{
		// 削除前にnullptrに置き換える（安全のため）
		for (auto& sphere : m_spheres)
		{
			if (sphere.object == targetObject)
			{
				sphere.object = nullptr;
			}
		}

		// 対応するオブジェクトを削除
		m_spheres.erase(std::remove_if(m_spheres.begin(), m_spheres.end(),
			[targetObject](const auto& collision) {
				return collision.object == targetObject || collision.object == nullptr;
			}),
			m_spheres.end());
	}
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

	// Collision毎に判定を描画

	// OBBの描画
	for (const auto& obb : m_obbs){
		DX::Draw(m_primitiveBatch.get(), *obb.collision, DirectX::Colors::Red);
	}

	// Sphereの描画
	for (const auto& sphere : m_spheres){
		DX::Draw(m_primitiveBatch.get(), *sphere.collision, DirectX::Colors::Blue);
	}

	// OBBのプロキシ球の描画
	for (auto& sphere : m_obbProxies){
		DX::Draw(m_primitiveBatch.get(), *sphere, DirectX::Colors::LimeGreen);
	}
	// 描画終了
	m_primitiveBatch->End();
}