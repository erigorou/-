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


const float Cudgel::CUDGEL_SCALE = Boss::BOSS_SCALE * 1.2f;						// ���_�̊g�嗦
const DirectX::SimpleMath::Vector3 Cudgel::DIRECTION_ENEMY = { 8.0f, 5.0f, 0.0f };	// ������̋����i�G�Ɣ�r�j
const DirectX::SimpleMath::Vector3 Cudgel::CUDGEL_LENGTH =	{ 0.0f, 35.0f, 0.0f };	// ���_�̒����i��ԉ�����j
const DirectX::SimpleMath::Vector3 Cudgel::CUDGEL_HADLE_POS = { 0.0f, 10.0f, 0.0f };	// ���_�̎����̕����i��ԏ�j


// --------------------------------
// �R���X�g���N�^
// --------------------------------
Cudgel::Cudgel(PlayScene* playScene)
	:
	m_playScene(playScene)
	,m_currentState()
	,m_worldMatrix(DirectX::SimpleMath::Matrix::Identity)
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

	// ���f�����擾����
	m_model = GameResources::GetInstance()->GetModel("cudgel");

	CreateState();			// �X�e�[�g�̍쐬
	CreateCollision();		// �����蔻��̐���

	// �C�x���g��o�^
	EventMessenger::AttachGetter("GetCudgelObject", std::bind(&Cudgel::GetCudgelObject, this));
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
	EventMessenger::Execute("AddOrientedCollision", &data);
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

	// �G�����Ȃ��ꍇ�͕`�悵�Ȃ�
	if (! GetPlayScene()->GetBoss()) return;

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
void Cudgel::ChangeState(IWeapon* state)
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



void Cudgel::HitAction(InterSectData data)
{
	m_currentState->HitAction(data);
}