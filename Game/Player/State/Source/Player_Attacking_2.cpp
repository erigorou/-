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

#include "Game/Player/Player.h"
#include "Game/Weapon/Sword/Sword.h"
#include "Game/Boss/Boss.h"
#include "Game/Player/State/Header/Player_Attacking_2.h"



// �R���X�g���N�^
PlayerAttacking_2::PlayerAttacking_2(Player* player)
	:
	m_player(player)
	, m_totalSeconds()
	, m_attackElapsedTime()
	, m_model()
{
}

// �f�X�g���N�^
PlayerAttacking_2::~PlayerAttacking_2()
{
}


// ����������
void PlayerAttacking_2::Initialize()
{


}

// ���O�X�V����
void PlayerAttacking_2::PreUpdate()
{
	// �o�ߎ��Ԃ̏�����
	m_totalSeconds = 0.f;

	// ������U����ԂɕύX
	m_player->GetPlayScene()->GetSword()->ChangeState(
		m_player->GetPlayScene()->GetSword()->GetAttacking_2State()
	);

	Sound::PlaySE(Sound::SE_TYPE::PLAYER_ATTACK);
}

// �X�V����
void PlayerAttacking_2::Update(const float& elapsedTime)
{
	m_totalSeconds += elapsedTime;

	UpdateAnimation();

	// ���Ԃ��v�����A��莞�Ԍo�߂ŃX�e�[�g��J��
	m_player->TimeComparison(m_totalSeconds, Player::APPLIED_ATTACK_TIME, m_player->GetPlayerIdlingState(), elapsedTime);
}


// �A�j���[�V�����̍X�V
void PlayerAttacking_2::UpdateAnimation()
{
	if (m_totalSeconds > Player::NORMAL_ATTACK_TIME) return;

	DirectX::SimpleMath::Vector3 currentAnimPos = DirectX::SimpleMath::Vector3::Zero;

	// �C�[�W���O�Ŏg�p���邽�߂̕ϐ� 0-1
	float t = m_totalSeconds / Player::NORMAL_ATTACK_TIME;

	// ��]�ʂ̌v�Z���s��
	float currentAngle = m_player->GetAngle();
	currentAnimPos.y = -40 + 80 * Easing::easeOutBack(t) + currentAngle;

	// radian�ɕϊ�
	currentAnimPos.y = DirectX::XMConvertToRadians(currentAnimPos.y);

	// �t�^
	m_player->SetAnimationRotate(currentAnimPos);

}


// �L�[����
void PlayerAttacking_2::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	if (key == DirectX::Keyboard::LeftShift && m_totalSeconds >= Player::X_COOL_TIME)	m_player->ChangeState(m_player->GetPlayerDodgingState());
}


void PlayerAttacking_2::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}



// ����X�V����
void PlayerAttacking_2::PostUpdate()
{
	// ������U����ԂɕύX
	m_player->GetPlayScene()->GetSword()->ChangeState(
		m_player->GetPlayScene()->GetSword()->GetIdlingState()
	);

	// �A�j���[�V������������
	m_player->SetAnimationRotate(DirectX::SimpleMath::Vector3::Zero);
}



// �I������
void PlayerAttacking_2::Finalize()
{
}
