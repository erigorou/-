// �����蔻������

#include "pch.h"
#include "CollisionManager.h"
#include "Interface/IObject.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Game/Messenger/EventMessenger.h"
#include "Libraries/Mylib/DebugDraw.h"

// -------------------------------------------------------
// �R���X�g���N�^
// -------------------------------------------------------
CollisionManager::CollisionManager()
	: m_basicEffect(nullptr)
	, m_inputLayout(nullptr)
	, m_primitiveBatch(nullptr)
	, m_obbs()
	, m_spheres()
	, m_drawFlag(false)

{
	// �����Ɠ����ɏ��������s��
	Initialize();
}



// -------------------------------------------------------
// �f�X�g���N�^
// -------------------------------------------------------
CollisionManager::~CollisionManager()
{
	Clear();
}



// -------------------------------------------------------
// ����������
// -------------------------------------------------------
void CollisionManager::Initialize()
{
	CommonResources* resources = CommonResources::GetInstance();

	auto device = resources->GetDeviceResources()->GetD3DDevice();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();

	// �x�[�V�b�N�G�t�F�N�g���쐬����
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);

	// ���̓��C�A�E�g���쐬����
	DX::ThrowIfFailed(
		DirectX::CreateInputLayoutFromEffect<DirectX::VertexPositionColor>(
			device,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf())
	);

	// �v���~�e�B�u�o�b�`�𐶐�
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(context);

	// �L�[�{�[�h���쐬����
	m_keyboardState = DirectX::Keyboard::Get().GetState();

	// �C�x���g�̓o�^
	AddEventMessenger();
}



// -------------------------------------------------------
// �C�x���g�̓o�^
// -------------------------------------------------------
void CollisionManager::AddEventMessenger()
{
	EventMessenger::Attach("AddOrientedCollision", std::bind(&CollisionManager::AddCollision<DirectX::BoundingOrientedBox>, this, std::placeholders::_1));
	EventMessenger::Attach("AddSphereCollision"	, std::bind(&CollisionManager::AddCollision<DirectX::BoundingSphere>, this, std::placeholders::_1));
}



// -------------------------------------------------------
// �X�V�֐�
// -------------------------------------------------------
void CollisionManager::Update()
{
	// OBB�̃v���L�V�Ƌ��̓����蔻��
	for (size_t i = 0; i < m_obbs.size(); i++)
	{
		// OBB�̃v���L�V���̒��S��OBB�̒��S�ɐݒ�
		m_obbProxies[i] = CreateProxySphere(m_obbs[i].collision);

		for (size_t j = 0; j < m_spheres.size(); j++)
		{
			// �v���L�V�ƏՓ˂��Ă��Ȃ���Ύ��̋��Ɉڂ�
			if (!m_obbProxies[i]->Intersects(*m_spheres[j].collision))	continue;

			// OBB�Ƌ��̓����蔻��
			if (m_obbs[i].collision->Intersects(*m_spheres[j].collision))
			{
				// �Փ˂����Ƃ��ɑ���ɓn���f�[�^���쐬
				InterSectData obbData		= { m_obbs[i]	.objType, m_obbs[i].colType,	m_obbProxies[i].get()};
				InterSectData sphereData	= { m_spheres[j].objType, m_spheres[i].colType, m_spheres[j].collision };

				// �Փ˂����Ƃ��̏������Ăяo��
				m_obbs[i].object->HitAction(sphereData);
				m_spheres[j].object->HitAction(obbData);
			}
		}
	}


	// �����m�ɂ�铖���蔻��
	for (size_t i = 0; i < m_spheres.size() - 1; i++)
	{
		for (size_t j = i + 1; j < m_spheres.size(); j++)
		{
			// �����m�̓����蔻��
			if (m_spheres[i].collision->Intersects(*m_spheres[j].collision))
			{
				// �Փ˂����Ƃ��ɑ���ɓn���f�[�^���쐬
				InterSectData sphereData1 = { m_spheres[i].objType, m_spheres[i].colType, m_spheres[i].collision};
				InterSectData sphereData2 = { m_spheres[j].objType, m_spheres[j].colType, m_spheres[j].collision};

				// �Փ˂����Ƃ��̏������Ăяo��
				m_spheres[i].object->HitAction(sphereData2);
				m_spheres[j].object->HitAction(sphereData1);
			}
		}
	}

	// �L�[�{�[�h�̏�Ԃ��擾����
	m_keyboardState = DirectX::Keyboard::Get().GetState();
	// �L�[�{�[�h�X�e�[�g�g���b�J�[���X�V����
	m_keyboardStateTracker.Update(m_keyboardState);


	// F5�L�[�������ꂽ��A�`��t���O��؂�ւ���
	if (m_keyboardStateTracker.IsKeyPressed(DirectX::Keyboard::F5))
	{
		m_drawFlag = !m_drawFlag;
	}
}


// -------------------------------------------------------
// �`��֐�
// -------------------------------------------------------
void CollisionManager::Render
	(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
	)
{
	if ( ! m_drawFlag) return;

	// �Փ˔���̕`��
	DrawCollision(view, projection);
}


// -------------------------------------------------------
// �����蔻����N���A
// -------------------------------------------------------
void CollisionManager::Clear()
{
	m_obbs.clear();
	m_spheres.clear();
}


// -------------------------------------------------------
// �Փ˔����ǉ�����
// -------------------------------------------------------
template<typename T>
void CollisionManager::AddCollision(void* args)
{
    // ���������҂���^�ɃL���X�g
    auto* collisionData = static_cast<CollisionData<T>*>(args);

    if (!collisionData) return; // �s���Ȉ����̏ꍇ�͏I��

    // �Փ˔���f�[�^��Ή�����R���e�i�ɒǉ�
    if constexpr (std::is_same_v<T, DirectX::BoundingOrientedBox>)
    {
        // OBB��ۑ�
        m_obbs.push_back(*collisionData);
        // OBB�̃v���L�V���𐶐�
        m_obbProxies.push_back(CreateProxySphere(static_cast<const DirectX::BoundingOrientedBox*>(collisionData->collision)));
    }
    else if constexpr (std::is_same_v<T, DirectX::BoundingSphere>)
    {
        // Sphere��ۑ�
        m_spheres.push_back(*collisionData);
    }
}


// -------------------------------------------------------
// OBB�̃v���L�V���𐶐�����
// -------------------------------------------------------
inline std::unique_ptr<DirectX::BoundingSphere> CollisionManager::CreateProxySphere(const DirectX::BoundingOrientedBox* collision)
{
	// BoundingSphere��OBB�̒��S�ƍő唼�a�ō쐬
	float radius = sqrtf(
		collision->Extents.x * collision->Extents.x +
		collision->Extents.y * collision->Extents.y +
		collision->Extents.z * collision->Extents.z
	);

	std::unique_ptr<DirectX::BoundingSphere> proxy = std::make_unique<DirectX::BoundingSphere>(collision->Center, radius);

	return std::move(proxy);
}

// -------------------------------------------------------
/// �����蔻����폜����
// -------------------------------------------------------
void CollisionManager::DeleteCollision(CollisionType collType, IObject* object)
{
	// �s����CollisionType�̏ꍇ�͏������I��
	if (collType != CollisionType::OBB && collType != CollisionType::Sphere)	return;

	// �I�u�W�F�N�g�폜������⏕�֐��Ƃ��ċ��ʉ�
	auto EraseMatchingObject = [object](auto& container) {
		container.erase(std::remove_if(container.begin(), container.end(),[object](const auto& collision)
			{
				return collision.object == object;
			}),
			container.end());
		};

	// CollisionType�ɉ����ēK�؂ȃR���e�i����폜
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
// �Փ˔���͈̔͂̕`��
// -------------------------------------------------------
inline void CollisionManager::DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = CommonResources::GetInstance()->GetCommonStates();

	// �`��ݒ���s��
	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthDefault(), 0);
	context->RSSetState(states->CullNone());
	context->IASetInputLayout(m_inputLayout.Get());
	// �r���[�s��Ǝˉe�s���ݒ�
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(projection);
	m_basicEffect->Apply(context);
	// �`��J�n
	m_primitiveBatch->Begin();
	
	// Collision���ɔ����`��

	for (auto obb : m_obbs)
	{
		DX::Draw(m_primitiveBatch.get(), *obb.collision, DirectX::Colors::Red);
	}

	for (auto sphere : m_spheres)
	{
		DX::Draw( m_primitiveBatch.get(), *sphere.collision, DirectX::Colors::Blue);
	}

	for (auto& sphere : m_obbProxies)
	{
		DX::Draw(m_primitiveBatch.get(), *sphere, DirectX::Colors::LimeGreen);
	}
	// �b���I��
	m_primitiveBatch->End();
}