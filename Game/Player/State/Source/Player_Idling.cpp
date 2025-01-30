#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Boss/Boss.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Sword/Sword.h"

#include "Game/Player/State/Header/Player_Idling.h"


// -------------------------------
// �R���X�g���N�^
// -------------------------------
PlayerIdling::PlayerIdling(Player* player)
	: m_player(player)
	, m_totalSeconds(0.0f)
{
}


// -------------------------------
// �f�X�g���N�^
// -------------------------------
PlayerIdling::~PlayerIdling()
{
}


// -------------------------------
// ����������
// -------------------------------
void PlayerIdling::Initialize()
{
}


// -------------------------------
// �ύX�����iin�j
// -------------------------------
void PlayerIdling::PreUpdate()
{
	// �o�ߎ��Ԃ̏�����
	m_totalSeconds = 0.0f;

	// �ړ����x�̏�����
	m_player->SetSpeed(DirectX::SimpleMath::Vector3::Zero);
	// �ړ������x�̏�����
	m_player->SetAcceleration(DirectX::SimpleMath::Vector3::Zero);
}


// -------------------------------
// �X�V����
// -------------------------------
void PlayerIdling::Update(const float& elapsedTime)
{
	// �o�ߎ��Ԃ̉��Z
	m_totalSeconds += elapsedTime;

	// �v���C���[�̌�����G�Ɍ�����
	DirectX::SimpleMath::Vector3 enemyPos = m_player->GetPlayScene()->GetTargetPosition();
	m_player->SetAngle(m_player->CalucratePlayerRotation(enemyPos));

	// �v���C���[�̈ړ�
	m_player->MovePlayer();
}


// -------------------------------
// �ύX�����iout�j
// -------------------------------
void PlayerIdling::PostUpdate()
{
}



// -------------------------------
// �L�[�{�[�h����(�������u��)
// -------------------------------
void PlayerIdling::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	if (key == DirectX::Keyboard::X			)	m_player->ChangeState(m_player->GetPlayerAttackingState1());
	if (key == DirectX::Keyboard::LeftShift	)	m_player->ChangeState(m_player->GetPlayerDodgingState());
}


// -------------------------------
// �L�[�{�[�h����(�����Ă��)
// -------------------------------
void PlayerIdling::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}



// -------------------------------
// �I������
// -------------------------------
void PlayerIdling::Finalize()
{
}
