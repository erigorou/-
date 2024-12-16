// 当たり判定やるやつ

#include "pch.h"
#include "CollisionManager.h"
#include "Interface/IObject.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/Mylib/DebugDraw.h"

// -------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// -------------------------------------------------------
CollisionManager::CollisionManager()
	: m_basicEffect(nullptr)
	, m_inputLayout(nullptr)
	, m_primitiveBatch(nullptr)
	, m_obbs()
	, m_spheres()
	, m_drawFlag(false)

{
	Initialize();
}



// -------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// -------------------------------------------------------
CollisionManager::~CollisionManager()
{
	Clear();
}



void CollisionManager::Initialize()
{
	CommonResources* resources = CommonResources::GetInstance();

	auto device = resources->GetDeviceResources()->GetD3DDevice();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();

	// ベーシックエフェクトを作成する
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);

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
}




// -------------------------------------------------------
/// <summary>
/// 更新関数
/// </summary>
// -------------------------------------------------------
void CollisionManager::Update()
{
	// OBBのプロキシと球の当たり判定 *
	for (size_t i = 0; i < m_obbs.size(); i++)
	{
		// OBBのプロキシ球の中心をOBBの中心に設定
		m_obbProxies[i] = CreateProxySphere(m_obbs[i].collision);

		for (size_t j = 0; j < m_spheres.size(); j++)
		{
			// プロキシと衝突していなければ次の球に移る
			if (!m_obbProxies[i].Intersects(*m_spheres[j].collision))	continue;

			// OBBと球の当たり判定
			if (m_obbs[i].collision->Intersects(*m_spheres[j].collision))
			{
				InterSectData obbData = { m_obbs[i].objType, m_obbs[i].colType,	m_obbs[i].object };
				InterSectData sphereData = { m_spheres[j].objType, m_spheres[i].colType,	m_spheres[j].object };

				m_obbs[i].object->HitAction(sphereData);
				m_spheres[j].object->HitAction(obbData);
			}
		}
	}


	// 球同士による当たり判定
	for (size_t i = 0; i < m_spheres.size() - 1; i++)
	{
		for (size_t j = i + 1; j < m_spheres.size(); j++)
		{
			/////////////////衝突した場合の当たり判定を比較する/////////////////////
			if (m_spheres[i].collision->Intersects(*m_spheres[j].collision))
			{
				InterSectData sphereData1 = { m_spheres[i].objType, m_spheres[i].colType, m_spheres[i].object};
				InterSectData sphereData2 = { m_spheres[j].objType, m_spheres[j].colType, m_spheres[j].object};

				m_spheres[i].object->HitAction(sphereData2);
				m_spheres[j].object->HitAction(sphereData1);
			}
		}
	}

	// キーボードの状態を取得する
	m_keyboardState = DirectX::Keyboard::Get().GetState();
	// キーボードステートトラッカーを更新する
	m_keyboardStateTracker.Update(m_keyboardState);


	// F7キーが押されたら、描画フラグを切り替える
	if (m_keyboardStateTracker.IsKeyPressed(DirectX::Keyboard::F5))
	{
		m_drawFlag = !m_drawFlag;
	}
}


void CollisionManager::Render
	(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
	)
{
	if ( ! m_drawFlag) return;

	// 衝突判定の描画
	DrawCollision(view, projection);
}




// -------------------------------------------------------
/// <summary>
/// 当たり判定をクリア
/// </summary>
// -------------------------------------------------------
void CollisionManager::Clear()
{
	m_obbs.clear();
	m_spheres.clear();
}


// -------------------------------------------------------
// 衝突判定を追加する
// -------------------------------------------------------
template</* OBB */>
void CollisionManager::AddCollision<DirectX::BoundingOrientedBox*>(ObjectType objType, CollisionType colType, IObject* obj, DirectX::BoundingOrientedBox* collision)
{
	// CollisionDataを生成してコンテナに追加
	m_obbs.emplace_back(objType, colType, obj, collision);

	// OBBのプロキシ球を生成してコンテナに追加
	m_obbProxies.push_back(CreateProxySphere(collision));
}

template</* Sphere */>
void CollisionManager::AddCollision<DirectX::BoundingSphere*>(ObjectType objType, CollisionType colType, IObject* obj, DirectX::BoundingSphere* collision)
{
	// CollisionDataを生成してコンテナに追加
	m_spheres.emplace_back(objType, colType, obj, collision);
}

// -------------------------------------------------------
// OBBのプロキシ球を生成する
// -------------------------------------------------------
inline DirectX::BoundingSphere CollisionManager::CreateProxySphere(const DirectX::BoundingOrientedBox* collision)
{
	// BoundingSphereをOBBの中心と最大半径で作成
	float radius = sqrtf(
		collision->Extents.x * collision->Extents.x +
		collision->Extents.y * collision->Extents.y +
		collision->Extents.z * collision->Extents.z
	);

	return DirectX::BoundingSphere(collision->Center, radius);
}

// -------------------------------------------------------
/// 当たり判定を削除する
// -------------------------------------------------------
void CollisionManager::DeleteCollision(CollisionType collType, IObject* object)
{
	// 不正なCollisionTypeの場合は処理を終了
	if (collType != CollisionType::OBB && collType != CollisionType::Sphere)	return;

	// オブジェクト削除処理を補助関数として共通化
	auto EraseMatchingObject = [object](auto& container) {
		container.erase(
			std::remove_if(container.begin(), container.end(),
				[object](const auto& collision) {
					return collision.object == object;
				}),
			container.end());
		};

	// CollisionTypeに応じて適切なコンテナから削除
	switch (collType)
	{
	case CollisionType::OBB:
		EraseMatchingObject(m_obbs);
		break;

	case CollisionType::Sphere:
		EraseMatchingObject(m_spheres);
		break;
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

	for (auto obb : m_obbs)
	{
		DX::Draw(m_primitiveBatch.get(), *obb.collision, DirectX::Colors::Red);
	}

	for (auto sphere : m_spheres)
	{
		DX::Draw( m_primitiveBatch.get(), *sphere.collision, DirectX::Colors::Blue);
	}

	for (auto sphere : m_obbProxies)
	{
		DX::Draw(m_primitiveBatch.get(),sphere,DirectX::Colors::LimeGreen);
	}
	m_primitiveBatch->End();
}



