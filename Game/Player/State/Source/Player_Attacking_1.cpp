/// ---------------------------------------
///
/// �v���C���[���U�������鏈��
///
/// ---------------------------------------

#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Sound/Sound.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/Weapon/WeaponState.h"

#include "Game/Player/Player.h"
#include "Game/Boss/Boss.h"
#include "Game/Player/State/Header/Player_Attacking_1.h"


// -----------------------
// �R���X�g���N�^
// -----------------------
PlayerAttacking_1::PlayerAttacking_1(Player* player)
	:
	m_player(player)
	, m_totalSeconds()
	, m_model()
{
}


// -----------------------
// �f�X�g���N�^
// -----------------------
PlayerAttacking_1::~PlayerAttacking_1()
{
}


// -----------------------
// ����������
// -----------------------
void PlayerAttacking_1::Initialize()
{
}


// -----------------------
// ���O�X�V����
// -----------------------
void PlayerAttacking_1::PreUpdate()
{
	// �o�ߎ��Ԃ̏�����
	m_totalSeconds = 0.0f;

	// ������U����ԂɕύX
	SwordState state = SwordState::Attack1;
	EventMessenger::Execute("ChangeSwordState", &state);

	// ���ʉ��̍Đ�
	Sound::PlaySE(Sound::SE_TYPE::PLAYER_ATTACK);
}


// -----------------------
// �X�V����
// -----------------------
void PlayerAttacking_1::Update(const float& elapsedTime)
{
	m_totalSeconds += elapsedTime;

	// �A�j���[�V�����̍X�V
	UpdateAnimation();

	// ���Ԃ��v�����A��莞�Ԍo�߂ŃX�e�[�g��J��
	m_player->TimeComparison(m_totalSeconds, Player::APPLIED_ATTACK_TIME, m_player->GetPlayerIdlingState(), elapsedTime);
}



// -----------------------
// �A�j���[�V�����X�V
// -----------------------
void PlayerAttacking_1::UpdateAnimation()
{
	if (m_totalSeconds > Player::NORMAL_ATTACK_TIME) return;

	DirectX::SimpleMath::Vector3 currentAnimPos = DirectX::SimpleMath::Vector3::Zero;

	// �C�[�W���O�Ŏg�p���邽�߂̕ϐ� 0-1
	float t = m_totalSeconds / Player::NORMAL_ATTACK_TIME;

	// ��]�ʂ̌v�Z���s��
	float currentAngle = m_player->GetAngle();
	currentAnimPos.y = 40 - 80 * Easing::easeOutExpo(t) + currentAngle;

	// radian�ɕϊ�
	currentAnimPos.y = DirectX::XMConvertToRadians(currentAnimPos.y);

	// �v���C���[�ɐݒ肷��
	m_player->SetAnimationRotate(currentAnimPos);
}


// -----------------------
// �L�[����
// -----------------------
void PlayerAttacking_1::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	if (key == DirectX::Keyboard::X && m_totalSeconds >= Player::X_COOL_TIME)	m_player->ChangeState(m_player->GetPlayerAttackingState2());

	// ���
	if (key == DirectX::Keyboard::LeftShift)
	{
		m_player->ChangeState(m_player->GetPlayerDodgingState());
	}
}

// -----------------------
// �L�[����
// -----------------------
void PlayerAttacking_1::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}


// -----------------------
// ����X�V
// -----------------------
void PlayerAttacking_1::PostUpdate()
{
	// �A�j���[�V�����̍X�V
	m_player->SetAnimationRotate(DirectX::SimpleMath::Vector3::Zero);

	// �����ҋ@��ԂɕύX
	SwordState state = SwordState::Idle;
	EventMessenger::Execute("ChangeSwordState", &state);
}


// -----------------------
// �I������
// -----------------------
void PlayerAttacking_1::Finalize()
{
}