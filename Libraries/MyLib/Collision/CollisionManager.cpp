// 当たり判定やるやつ

#include "pch.h"
#include "CollisionManager.h"
#include "Interface/IObject.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/Microsoft/DebugDraw.h"

// -------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// -------------------------------------------------------
CollisionManager::CollisionManager()
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
}



// -------------------------------------------------------
/// <summary>
/// 更新関数
/// </summary>
// -------------------------------------------------------
void CollisionManager::Update()
{
	// 4分割で当たるかどうか

	 //////////////////OBBと球の当たり判定/////////////////////
    for (size_t i = 0; i < m_obbs.size(); i++)
    {
        for (size_t j = 0; j < m_spheres.size(); j++)
        {
			/////////////////衝突した場合の当たり判定を比較する/////////////////////
            if (m_obbs[i].obb->Intersects(*m_spheres[j].sphere))
            {
				InterSectData obbData	= { m_obbs[i]	.objType, m_obbs[i].colType,	m_obbs[i].object	};
				InterSectData sphereData= { m_spheres[j].objType, m_spheres[i].colType,	m_spheres[j].object	};

				m_obbs[i]	.object->HitAction(sphereData);
				m_spheres[j].object->HitAction(obbData);
            }
        }
    }


	/////////////////球同士の当たり判定////////////////////////
	for (size_t i = 0; i < m_spheres.size() - 1; i++)
	{
		for (size_t j = i + 1; j < m_spheres.size(); j++)
		{
			/////////////////衝突した場合の当たり判定を比較する/////////////////////
			if (m_spheres[i].sphere->Intersects(*m_spheres[j].sphere))
			{
				InterSectData sphereData1 = { m_spheres[i].objType, m_spheres[i].colType, m_spheres[i].object};
				InterSectData sphereData2 = { m_spheres[j].objType, m_spheres[j].colType, m_spheres[j].object};

				m_spheres[i].object->HitAction(sphereData2);
				m_spheres[j].object->HitAction(sphereData1);
			}
		}
	}
}




void CollisionManager::Render
	(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
	)
{
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = CommonResources::GetInstance()->GetCommonStates();

	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthDefault(), 0);
	context->RSSetState(states->CullNone());
	context->IASetInputLayout(m_inputLayout.Get());
	//** デバッグドローでは、ワールド変換いらない
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(projection);
	m_basicEffect->Apply(context);

	m_primitiveBatch->Begin();

	for(auto obb : m_obbs)
	{
		DX::Draw
		(
			m_primitiveBatch.get(),
			*obb.obb,
			DirectX::Colors::Red
		);
	}

	for (auto sphere : m_spheres)
	{
		DX::Draw
		(
			m_primitiveBatch.get(),
			*sphere.sphere,
			DirectX::Colors::Blue
		);
	}
	m_primitiveBatch->End();
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
/// <summary>
/// 当たり判定一覧から指定したオブジェクトの当たり判定を削除
/// </summary>
/// <param name="object">オブジェクトのポインタをIDとして使用</param>
// -------------------------------------------------------
void CollisionManager::DeleteOBBCollision(IObject* object)
{
	// m_obbsからobjectを削除
	m_obbs.erase(std::remove_if(m_obbs.begin(), m_obbs.end(),
		[object](const OBBCollision& obbCollision) { return obbCollision.object == object; }),
		m_obbs.end());
}



// -------------------------------------------------------
/// <summary>
/// 当たり判定一覧から指定したオブジェクトの当たり判定を削除
/// </summary>
/// <param name="object">オブジェクトのポインタをIDとして使用</param>
// -------------------------------------------------------
void CollisionManager::DeleteSphereCollision(IObject* object)
{
	// m_spheresからobjectを削除
	m_spheres.erase(std::remove_if(m_spheres.begin(), m_spheres.end(),
		[object](const SphereCollision& sphereCollision) { return sphereCollision.object == object; }),
		m_spheres.end());
}




inline void CollisionManager::DrawOBBCollision()
{
}
