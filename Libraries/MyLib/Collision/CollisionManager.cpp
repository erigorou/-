// 当たり判定やるやつ

#include "pch.h"
#include "CollisionManager.h"
#include "Interface/IObject.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Game/Messenger/EventMessenger.h"
#include "Libraries/Mylib/DebugDraw.h"

// -------------------------------------------------------
// コンストラクタ
// -------------------------------------------------------
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
}

// -------------------------------------------------------
// デストラクタ
// -------------------------------------------------------
CollisionManager::~CollisionManager()
{
	Clear();
}

// -------------------------------------------------------
// 初期化処理
// -------------------------------------------------------
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

// -------------------------------------------------------
// イベントの登録
// -------------------------------------------------------
void CollisionManager::AddEventMessenger()
{
	// OBBの登録
	EventMessenger::Attach(EventList::AddOBBCollision, std::bind(&CollisionManager::AddCollision<DirectX::BoundingOrientedBox>, this, std::placeholders::_1));
	// Sphereの登録
	EventMessenger::Attach(EventList::AddSphereCollision, std::bind(&CollisionManager::AddCollision<DirectX::BoundingSphere>, this, std::placeholders::_1));
	// 衝突判定の削除
	EventMessenger::Attach(EventList::DeleteCollision, std::bind(&CollisionManager::DeleteCollision, this, std::placeholders::_1));
}

// -------------------------------------------------------
// 更新関数
// -------------------------------------------------------
void CollisionManager::Update()
{
	// OBBのプロキシと球の当たり判定
	for (int i = 0; i < static_cast<int>(m_obbs.size()); i++)
	{
		// OBBのプロキシ球の中心をOBBの中心に設定
		m_obbProxies[i] = CreateProxySphere(m_obbs[i].collision);

		for (int j = 0; j < static_cast<int>(m_spheres.size()); j++)
		{
			// プロキシと衝突していなければ次の球に移る
			if (!m_obbProxies[i]->Intersects(*m_spheres[j].collision))	continue;

			// OBBと球の当たり判定
			if (m_obbs[i].collision->Intersects(*m_spheres[j].collision))
			{
				// 衝突したときに相手に渡すデータを作成
				InterSectData obbData = { m_obbs[i].objType, m_obbs[i].colType,	m_obbProxies[i].get() };
				InterSectData sphereData = { m_spheres[j].objType, m_spheres[i].colType, m_spheres[j].collision };

				// 衝突したときの処理を呼び出す
				m_obbs[i].object->HitAction(sphereData);
				m_spheres[j].object->HitAction(obbData);
			}
		}
	}

	// 球同士による当たり判定
	for (int i = 0; i < static_cast<int>(m_spheres.size() - 1); i++)
	{
		for (int j = i + 1; j < static_cast<int>(m_spheres.size()); j++)
		{
			// 球同士の当たり判定
			if (m_spheres[i].collision->Intersects(*m_spheres[j].collision))
			{
				// 衝突したときに相手に渡すデータを作成
				InterSectData sphereData1 = { m_spheres[i].objType, m_spheres[i].colType, m_spheres[i].collision };
				InterSectData sphereData2 = { m_spheres[j].objType, m_spheres[j].colType, m_spheres[j].collision };

				// 衝突したときの処理を呼び出す
				m_spheres[i].object->HitAction(sphereData2);
				m_spheres[j].object->HitAction(sphereData1);
			}
		}
	}

	// キーボードの状態を取得する
	m_keyboardState = DirectX::Keyboard::Get().GetState();
	// キーボードステートトラッカーを更新する
	m_keyboardStateTracker.Update(m_keyboardState);

	// F5キーが押されたら、描画フラグを切り替える
	if (m_keyboardStateTracker.IsKeyPressed(DirectX::Keyboard::F5))
	{
		m_drawFlag = !m_drawFlag;
	}
}

// -------------------------------------------------------
// 描画関数
// -------------------------------------------------------
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

// -------------------------------------------------------
// 当たり判定をクリア
// -------------------------------------------------------
void CollisionManager::Clear()
{
	m_obbs.clear();
	m_spheres.clear();
}

// -------------------------------------------------------
// 衝突判定を追加する
// -------------------------------------------------------
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

// -------------------------------------------------------
// OBBのプロキシ球を生成する
// -------------------------------------------------------
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

// -------------------------------------------------------
/// 当たり判定を削除する
// -------------------------------------------------------
void CollisionManager::DeleteCollision(void* args)
{
	// argsはDeleteCollisionData構造体へのポインタ
	auto* deleteData = static_cast<DeleteCollisionData*>(args);

	// 不正な引数の場合は終了
	if (!deleteData) return;

	// OBBの場合はOBBとプロキシ球の両方を削除
	if (deleteData->collType == CollisionType::OBB)
	{
		// 対応するオブジェクトを削除するラムダ式
		auto EraseMatchingObject = [object = deleteData->object](auto& container)
			{
				container.erase(std::remove_if(container.begin(), container.end(), [object](const auto& collision)
					{
						return collision.object == object;	// オブジェクトが一致するか判定
					}),
					container.end());
			};

		// OBBのコンテナから削除
		EraseMatchingObject(m_obbs);
	}

	// Sphereの場合はSphereのみ削除
	else if (deleteData->collType == CollisionType::Sphere)
	{
		// 対応するオブジェクトを削除するラムダ式
		auto EraseMatchingObject = [object = deleteData->object](auto& container)
			{
				container.erase(std::remove_if(container.begin(), container.end(), [object](const auto& collision)
					{
						return collision.object == object;
					}),
					container.end());
			};

		// Sphereのコンテナから削除
		EraseMatchingObject(m_spheres);
	}
}

// -------------------------------------------------------
// 衝突判定の範囲の描画
// -------------------------------------------------------
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

	for (const auto& obb : m_obbs)
	{
		DX::Draw(m_primitiveBatch.get(), *obb.collision, DirectX::Colors::Red);
	}

	for (const auto& sphere : m_spheres)
	{
		DX::Draw(m_primitiveBatch.get(), *sphere.collision, DirectX::Colors::Blue);
	}

	for (auto& sphere : m_obbProxies)
	{
		DX::Draw(m_primitiveBatch.get(), *sphere, DirectX::Colors::LimeGreen);
	}
	// 秒が終了
	m_primitiveBatch->End();
}