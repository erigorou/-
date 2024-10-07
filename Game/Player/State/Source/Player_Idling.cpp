#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Sword/Sword.h"

#include "Game/Player/State/Header/Player_Idling.h"

// �R���X�g���N�^
PlayerIdling::PlayerIdling(Player* player)
	:
	 m_player(player)
	,m_totalSeconds()
	,m_model()
{
}


// �f�X�g���N�^
PlayerIdling::~PlayerIdling()
{
}


// ����������
void PlayerIdling::Initialize(DirectX::Model* model)
{
	// ���f�����擾����
	m_model = model;
}


// ���O�X�V����
void PlayerIdling::PreUpdate()
{
	// �o�ߎ��Ԃ̏�����
	m_totalSeconds = 0.0f;

	m_player->SetSpeed(DirectX::SimpleMath::Vector3::Zero);
	m_player->SetAcceleration(DirectX::SimpleMath::Vector3::Zero);
}


// �X�V����
void PlayerIdling::Update(const float& elapsedTime,  DirectX::SimpleMath::Vector3& parentPos)
{
	// elapsedTime�g��Ȃ����ǃG���[�o���Ȃ��ł˂��ĕ�
	UNREFERENCED_PARAMETER(elapsedTime);

	// �L�[�{�[�h�̓��͂��擾����
	DirectX::Keyboard::State keyboardState = DirectX::Keyboard::Get().GetState();

	// �v���C���[�̈ړ�
	m_player->MovePlayer();

	// ���V�t�g�̓��́@&& �ړ����Ȃ�
	if (keyboardState.LeftShift && m_player->GetVelocity() != DirectX::SimpleMath::Vector3::Zero)
	{
		m_player->ChangeState(m_player->GetPlayerDodgingState());
	}
	// X�̓���
	if (keyboardState.X) 
	{
		m_player->ChangeState(m_player->GetPlayerAttackingState1());
	}
}



// ����X�V����
void PlayerIdling::PostUpdate()
{
	// �C���_������΂����ɋL��
}


// �`�揈��
void PlayerIdling::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
	)
{
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(states);
	UNREFERENCED_PARAMETER(view);
	UNREFERENCED_PARAMETER(projection);
	UNREFERENCED_PARAMETER(m_model);

	//// �R�������\�[�X���擾����
	//CommonResources* resources = CommonResources::GetInstance();
}


// �I������
void PlayerIdling::Finalize()
{
}
