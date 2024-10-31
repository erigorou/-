// �����蔻������

#include "pch.h"
#include "CollisionManager.h"
#include "Interface/IObject.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/Microsoft/DebugDraw.h"

// -------------------------------------------------------
/// <summary>
/// �R���X�g���N�^
/// </summary>
// -------------------------------------------------------
CollisionManager::CollisionManager()
{
	Initialize();
}



// -------------------------------------------------------
/// <summary>
/// �f�X�g���N�^
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

	// �x�[�V�b�N�G�t�F�N�g���쐬����
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);

	DX::ThrowIfFailed(
		DirectX::CreateInputLayoutFromEffect<DirectX::VertexPositionColor>(
			device,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf())
	);

	// �v���~�e�B�u�o�b�`�𐶐�
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(context);
}



// -------------------------------------------------------
/// <summary>
/// �X�V�֐�
/// </summary>
// -------------------------------------------------------
void CollisionManager::Update()
{
	// 4�����œ����邩�ǂ���

	 //////////////////OBB�Ƌ��̓����蔻��/////////////////////
    for (size_t i = 0; i < m_obbs.size(); i++)
    {
        for (size_t j = 0; j < m_spheres.size(); j++)
        {
			/////////////////�Փ˂����ꍇ�̓����蔻����r����/////////////////////
            if (m_obbs[i].obb->Intersects(*m_spheres[j].sphere))
            {
				InterSectData obbData	= { m_obbs[i]	.objType, m_obbs[i].colType,	m_obbs[i].object	};
				InterSectData sphereData= { m_spheres[j].objType, m_spheres[i].colType,	m_spheres[j].object	};

				m_obbs[i]	.object->HitAction(sphereData);
				m_spheres[j].object->HitAction(obbData);
            }
        }
    }


	/////////////////�����m�̓����蔻��////////////////////////
	for (size_t i = 0; i < m_spheres.size() - 1; i++)
	{
		for (size_t j = i + 1; j < m_spheres.size(); j++)
		{
			/////////////////�Փ˂����ꍇ�̓����蔻����r����/////////////////////
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
	//** �f�o�b�O�h���[�ł́A���[���h�ϊ�����Ȃ�
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
/// �����蔻����N���A
/// </summary>
// -------------------------------------------------------
void CollisionManager::Clear()
{
	m_obbs.clear();
	m_spheres.clear();
}


// -------------------------------------------------------
/// <summary>
/// �����蔻��ꗗ����w�肵���I�u�W�F�N�g�̓����蔻����폜
/// </summary>
/// <param name="object">�I�u�W�F�N�g�̃|�C���^��ID�Ƃ��Ďg�p</param>
// -------------------------------------------------------
void CollisionManager::DeleteOBBCollision(IObject* object)
{
	// m_obbs����object���폜
	m_obbs.erase(std::remove_if(m_obbs.begin(), m_obbs.end(),
		[object](const OBBCollision& obbCollision) { return obbCollision.object == object; }),
		m_obbs.end());
}



// -------------------------------------------------------
/// <summary>
/// �����蔻��ꗗ����w�肵���I�u�W�F�N�g�̓����蔻����폜
/// </summary>
/// <param name="object">�I�u�W�F�N�g�̃|�C���^��ID�Ƃ��Ďg�p</param>
// -------------------------------------------------------
void CollisionManager::DeleteSphereCollision(IObject* object)
{
	// m_spheres����object���폜
	m_spheres.erase(std::remove_if(m_spheres.begin(), m_spheres.end(),
		[object](const SphereCollision& sphereCollision) { return sphereCollision.object == object; }),
		m_spheres.end());
}




inline void CollisionManager::DrawOBBCollision()
{
}
