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
#include "Game/GameResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/Collision.h"
#include "Game/Messenger/EventMessenger.h"
#include "Sword.h"
#include "Game/Player/Player.h"


// �Œ�l
const float Sword::SWORD_SCALE = Player::PLAYER_SCALE * 1.5f;


// �R���X�g���N�^
Sword::Sword(Player* player)
	:
	m_player(player),
	m_worldMatrix{ DirectX::SimpleMath::Matrix::Identity },
	m_collision{},
	m_originalBox{},
	m_canAttack{false}
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


	// ���f���̓ǂݍ���
	m_model = GameResources::GetInstance()->GetModel("sword");

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
	// �����蔻����쐬
	CreateCollision();

	// ���[���h���W�̏�����
	m_worldMatrix *= DirectX::SimpleMath::Matrix::CreateScale(SWORD_SCALE);

}


// �V�[���𐶐�����
void Sword::CreateState()
{
	// �X�e�[�g�𐶐�����
	m_swordIdling		= std::make_unique<Sword_Idling>(this);
	m_swordAttacking_1	= std::make_unique<Sword_Attacking_1>(this);
	m_swordAttacking_2	= std::make_unique<Sword_Attacking_2>(this);

	// �X�e�[�g������������
	m_swordIdling->Initialize();
	m_swordAttacking_1->Initialize();
	m_swordAttacking_2->Initialize();

	// ���݂̃X�e�[�g��ݒ�
	m_currentState = m_swordIdling.get();

	// �ҋ@�X�e�[�g���i�[
	m_states.push_back(m_swordIdling.get());
	// �U���X�e�[�g���i�[
	m_states.push_back(m_swordAttacking_1.get());
	// �U���X�e�[�g���i�[
	m_states.push_back(m_swordAttacking_2.get());
}


// --------------------------------------------
// �Փ˔���̐���
// --------------------------------------------
void Sword::CreateCollision()
{
	// ���f���̑傫������Փ˔�����擾����
	m_originalBox = Collision::Get_BoundingOrientedBox_FromMODEL(m_model);
	m_collision = std::make_unique<DirectX::BoundingOrientedBox>(m_originalBox);

	// �Փ˃f�[�^�̍쐬
	CollisionData<DirectX::BoundingOrientedBox> data =
	{
		ObjectType::Sword,		// �I�u�W�F�N�g�̎��
		CollisionType::OBB,		// �����蔻��̎��
		this,					// ���̃N���X�̃|�C���^
		m_collision.get()		// �����蔻��
	};

	// �Փ˔����Manager�ɓo�^
	EventMessenger::Execute("AddOrientedCollision", &data);
}


// --------------------------------------------
// �X�V����
// --------------------------------------------
void Sword::Update(float elapsedTime)
{
	// ���݂̃X�e�[�g�̍X�V����
	m_currentState->Update(elapsedTime);
}


// --------------------------------------------
// �`�揈��
// --------------------------------------------
void Sword::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
)
{
	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	// ���f����`�悷��
	m_model->Draw(context, *states, m_worldMatrix, view, projection);
}


// --------------------------------------------
// �I������
// --------------------------------------------
void Sword::Finalize()
{
}


// --------------------------------------------
// �X�e�[�g�̕ύX
// --------------------------------------------
void Sword::ChangeState(void* state)
{
	// index���擾
	int index = *static_cast<int*>(state);
	// ���݂Ɠ����X�e�[�g�ɂ͕ύX���Ȃ�
	if (m_currentState == m_states[index])return;
	// �X�e�[�g�̎��㏈��
	m_currentState->PostUpdate();
	// �V�����X�e�[�g�ɐ؂�ւ���
	m_currentState = m_states[index];
	// �V�����X�e�[�g�̎��O�������s��
	m_currentState->PreUpdate();
}


// --------------------------------------------
// ���������Ƃ��̏���
// --------------------------------------------
void Sword::HitAction(InterSectData data)
{
	m_currentState->HitAction(data);
}
