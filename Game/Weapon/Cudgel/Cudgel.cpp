// ----------------
//
// �S�̋��_
//
// ----------------

#include "pch.h"
#include <Model.h>
#include <cassert>
// �w�b�_�[
#include "Game/CommonResources.h"	
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "DeviceResources.h"
#include "Game/GameResources.h"
#include "Game/GameResources.h"
#include "Libraries/MyLib/Collision.h"
#include "Game/Messenger/EventMessenger.h"

#include "Game/Boss/Boss.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "Game/Boss/Boss.h"

#include "header/Cudgel_Idling.h"		// �ҋ@
#include "header/Cudgel_Attacking.h"	// �U��
#include "header/Cudgel_Sweeping.h"		// �ガ����

const float Cudgel::CUDGEL_SCALE = Boss::BOSS_SCALE * 1.2f;						// ���_�̊g�嗦
const DirectX::SimpleMath::Vector3 Cudgel::DIRECTION_ENEMY = { 8.0f, 5.0f, 0.0f };	// ������̋����i�G�Ɣ�r�j
const DirectX::SimpleMath::Vector3 Cudgel::CUDGEL_LENGTH =	{ 0.0f, 35.0f, 0.0f };	// ���_�̒����i��ԉ�����j
const DirectX::SimpleMath::Vector3 Cudgel::CUDGEL_HADLE_POS = { 0.0f, 10.0f, 0.0f };	// ���_�̎����̕����i��ԏ�j


// --------------------------------
// �R���X�g���N�^
// --------------------------------
Cudgel::Cudgel(Boss* boss)
	:
	m_boss(boss),
	m_currentState(),
	m_worldMatrix(DirectX::SimpleMath::Matrix::Identity)
{
}


// --------------------------------
// �f�X�g���N�^
// --------------------------------
Cudgel::~Cudgel()
{
}


// --------------------------------
// ����������
// --------------------------------
void Cudgel::Initialize()
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
	// �v���~�e�B�u�o�b�`�̍쐬
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(context);

	// ���_�̃|�C���^���擾
	EventMessenger::AttachGetter(GetterList::GetCudgel, std::bind(&Cudgel::GetCudgelObject, this));
	// ���_�̃X�e�[�g��ύX
	EventMessenger::Attach(EventList::ChangeCudgelState, std::bind(&Cudgel::ChangeState, this, std::placeholders::_1));

	// ���f�����擾����
	m_model = GameResources::GetInstance()->GetModel("cudgel");

	CreateState();			// �X�e�[�g�̍쐬
	CreateCollision();		// �����蔻��̐���
}


// --------------------------------
// ��Ԃ̐���
// --------------------------------
void Cudgel::CreateState()
{
	// ��Ԃ̐���
	m_idling	= std::make_unique<Cudgel_Idling>	(this);		// �ҋ@
	m_attacking = std::make_unique<Cudgel_Attacking>(this);		// �U��
	m_sweeping	= std::make_unique<Cudgel_Sweeping>	(this);		// �ガ����

	// ��Ԃ̏�����
	m_idling->		Initialize();	// �ҋ@
	m_attacking->	Initialize();	// �U��
	m_sweeping->	Initialize();	// �ガ����

	// ������Ԃ��w��
	m_currentState = m_idling.get();

	// �X�e�[�g�̃��X�g�ɒǉ�
	m_states.push_back(m_idling.get());
	m_states.push_back(m_attacking.get());
	m_states.push_back(m_sweeping.get());
}


// --------------------------------
// �����蔻��̐���
// --------------------------------
void Cudgel::CreateCollision()
{
	m_originalBox = Collision::Get_BoundingOrientedBox_FromMODEL(m_model);
	m_collision = std::make_unique<DirectX::BoundingOrientedBox>(m_originalBox);

	// �Փ˃f�[�^�̍쐬
	CollisionData<DirectX::BoundingOrientedBox> data =
	{
		ObjectType::Cudgel,		// �I�u�W�F�N�g�̎��
		CollisionType::OBB,		// �����蔻��̎��
		this,					// ���̃N���X�̃|�C���^
		m_collision.get()		// �����蔻��
	};

	// �����蔻����L�^����
	EventMessenger::Execute(EventList::AddOBBCollision, &data);
}


// --------------------------------
// �X�V����
// --------------------------------
void Cudgel::Update(float elapsedTime)
{
	// ���݂̃X�e�[�g�̍X�V����
	m_currentState->Update(elapsedTime);
}

// --------------------------------
// �`�揈��
// --------------------------------
void Cudgel::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
)
{
	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	// ���f����`��
	m_model->Draw(context, *states, m_worldMatrix, view, projection);

#ifdef _DEBUG
#endif // _DEBUG
}


// --------------------------------
// �I������
// --------------------------------
void Cudgel::Finalize()
{
}


// --------------------------------
// �X�e�[�g�̕ύX
// --------------------------------
void Cudgel::ChangeState(void* state)
{
	int index = *static_cast<int*>(state);

	// �X�e�[�g�̕ύX
	if (m_currentState == m_states[index]) return;

	// �X�e�[�g�̎��㏈��
	m_currentState->PostUpdate();
	// �V�����X�e�[�g�ɐ؂�ւ���
	m_currentState = m_states[index];
	// �V�����X�e�[�g�̎��O�������s��
	m_currentState->PreUpdate();
}



void Cudgel::HitAction(InterSectData data)
{
	m_currentState->HitAction(data);
}