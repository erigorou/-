// ----------------
//
// �����Y�̓�
//
// ----------------

#include "pch.h"
#include <Model.h>
#include <cassert>
// �w�b�_�[
#include "Game/CommonResources.h"	
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/Microsoft/DebugDraw.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/Collision.h"
#include "Sword.h"
#include "Game/Player/Player.h"


// �Œ�l
const float Sword::SWORD_SCALE = Player::PLAYER_SCALE * 1.5f;


// �R���X�g���N�^
Sword::Sword(PlayScene* playScene)
	:
	  m_playScene(playScene)
{
}


// �f�X�g���N�^
Sword::~Sword()
{
}


// ����������
void Sword::Initialize()
{
	CommonResources* resources = CommonResources::GetInstance();

	auto device = resources->GetDeviceResources()->GetD3DDevice();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();

	// ���f���𐶐�
	m_model = std::make_unique<DirectX::Model>();

	// ���f����ǂݍ��ޏ���
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// ���f����ǂݍ���
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Weapon/Sword/sword.cmo", *fx);


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
	// �v���~�e�B�u�o�b�`�̍쐬
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(context);

	// �X�e�[�g���쐬
	CreateState();

	CreateCollision();

	// ���[���h���W�̏�����
	m_worldMatrix *= DirectX::SimpleMath::Matrix::CreateScale(SWORD_SCALE);

}


// �V�[���𐶐�����
void Sword::CreateState()
{
	// �X�e�[�g�𐶐�����
	m_swordIdling		= std::make_unique<Sword_Idling>		(this);
	m_swordAttacking_1	= std::make_unique<Sword_Attacking_1>	(this);
	m_swordAttacking_2	= std::make_unique<Sword_Attacking_2>	(this);
	m_swordAttacking_3	= std::make_unique<Sword_Attacking_3>	(this);
	m_swordAttacking_4	= std::make_unique<Sword_Attacking_4>	(this);


	// �X�e�[�g������������
	m_swordIdling->Initialize();
	m_swordAttacking_1->Initialize();
	m_swordAttacking_2->Initialize();
	m_swordAttacking_3->Initialize();
	m_swordAttacking_4->Initialize();


	// ���݂̃X�e�[�g��ݒ�
	m_currentState = m_swordIdling.get();
}


/// <summary>
/// �����蔻��̐���
/// </summary>
void Sword::CreateCollision()
{
	m_originalBox = Collision::Get_BoundingOrientedBox_FromMODEL(m_model.get());
	m_collision = std::make_unique<DirectX::BoundingOrientedBox>(m_originalBox);

	// �����蔻����L�^����
	m_playScene->GetCollisionManager()->AddCollision(
		ObjectType::Sword,		// �I�u�W�F�N�g�̎��
		CollisionType::OBB,		// �����蔻��̎��
		this,					// ���̃N���X�̃|�C���^
		m_collision.get()		// �����蔻��
	);

	m_originalBox.Center;
}


// --------------------------------------------
/// <summary>
/// �����Y�̓��̍X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
// --------------------------------------------
void Sword::Update(float elapsedTime)
{
	// ���݂̃X�e�[�g�̍X�V����
	m_currentState->Update(elapsedTime);
}


// �`�揈��
void Sword::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	// ���݂̃X�e�[�g�̕`�揈��
	m_currentState->Render(context,states,view,projection);


#ifdef _DEBUG
	auto debugString = resources->GetDebugString();
	debugString->AddString("");
#endif // _DEBUG
}

// �I������
void Sword::Finalize()
{
}


// �V�[���ύX����
void Sword::ChangeState(IWeapon* state)
{
	// �����X�e�[�g�ɕύX���悤�Ƃ����瑁�����^�[��
	if (m_currentState == state)return;
	// �X�e�[�g�̎��㏈��
	m_currentState->PostUpdate();
	// �V�����X�e�[�g�ɐ؂�ւ���
	m_currentState = state;
	// �V�����X�e�[�g�̎��O�������s��
	m_currentState->PreUpdate();
}

void Sword::HitAction(InterSectData data)
{
	m_currentState->HitAction(data);
}
