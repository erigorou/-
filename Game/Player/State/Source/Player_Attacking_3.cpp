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
#include "Game/Enemy/Enemy.h"
#include "Game/Player/State/Header/Player_Attacking_3.h"


// �R���X�g���N�^
PlayerAttacking_3::PlayerAttacking_3(Player* player)
	:
	 m_player(player)
	,m_totalSeconds()
	,m_model()
{
}

// �f�X�g���N�^
PlayerAttacking_3::~PlayerAttacking_3()
{
}


// ����������
void PlayerAttacking_3::Initialize(DirectX::Model* model)
{
	// ���f�����擾����
	m_model = model;
}

// ���O�X�V����
void PlayerAttacking_3::PreUpdate()
{
	// �o�ߎ��Ԃ̏�����
	m_totalSeconds = 0.f;

	// ���̍U����ԂɕύX
	m_player->GetPlayScene()->GetSword()->ChangeState(
		m_player->GetPlayScene()->GetSword()->GetAttacking_3State()
	);

	Sound::PlaySE(Sound::SE_TYPE::PLAYER_ATTACK);

	m_position = m_player->GetPosition();
}

// �X�V����
void PlayerAttacking_3::Update(const float& elapsedTime)
{
	m_totalSeconds += elapsedTime;

	// �A�j���[�V�����̍X�V
	UpdateAnimation();


	// ���Ԃ��v�����A��莞�Ԍo�߂ŃX�e�[�g��J��
	m_player->TimeComparison(m_totalSeconds, Player::APPLIED_ATTACK_TIME, m_player->GetPlayerIdlingState(), elapsedTime);
}


// �A�j���[�V�����̍X�V
void PlayerAttacking_3::UpdateAnimation()
{
	float t = 0;

	// �؂�グ���[�V������
	if (m_totalSeconds < Player::NORMAL_ATTACK_TIME)
	{
		DirectX::SimpleMath::Vector3 currentAnimPos = DirectX::SimpleMath::Vector3::Zero;

		// �C�[�W���O�Ŏg�p���邽�߂̕ϐ� 0-1
		t = m_totalSeconds / Player::NORMAL_ATTACK_TIME;

		// ��]�ʂ̌v�Z���s��
		float currentAngle = m_player->GetAngle();
		currentAnimPos.x = -20 * Easing::easeOutElastic(t) + currentAngle;

		// radian�ɕϊ�
		currentAnimPos.x = DirectX::XMConvertToRadians(currentAnimPos.x);

		// �v���C���[�ɐݒ肷��
		m_player->SetAnimationRotate(currentAnimPos);
	}


	// �C�[�W���O�Ŏg�p���邽�߂̕ϐ� 0 - 1
	t = m_totalSeconds / Player::APPLIED_ATTACK_TIME;

	float JUMP_VALUE = 6.0f;

	// �C�[�W���O�̌o�ߎ��Ԃ��T�C���g�Ƃ��Ĉ���
	t = Math::CalculatingSinWave(t / 2, 1.0f, 1.0f);

	// 0�ȉ��ɂȂ�Ȃ��悤�ɃC�[�W���O�����
	float jump = std::max(JUMP_VALUE * Easing::easeOutCubic(t), 0.0f);

	// �v���C���[�ɐݒ肷��
	m_player->SetPosition(m_position + DirectX::SimpleMath::Vector3(0, jump, 0));
}



// �L�[����
void PlayerAttacking_3::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	if (key == DirectX::Keyboard::X			&& m_totalSeconds >= Player::X_COOL_TIME)	m_player->ChangeState(m_player->GetPlayerAttackingState4());
}



void PlayerAttacking_3::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}


// ����X�V����
void PlayerAttacking_3::PostUpdate()
{
	// ������U����ԂɕύX
	m_player->GetPlayScene()->GetSword()->ChangeState(
		m_player->GetPlayScene()->GetSword()->GetIdlingState()
	);

	// �A�j���[�V������������
	m_player->SetAnimationRotate(DirectX::SimpleMath::Vector3::Zero);
}


// �`�揈��
void PlayerAttacking_3::Render(
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



	// �R�������\�[�X���擾����
	CommonResources* resources = CommonResources::GetInstance();


	// �f�o�b�O�����uDebugString�v�ŕ\������
	auto debugString = resources->GetDebugString();
	debugString->AddString("");
}


// �I������
void PlayerAttacking_3::Finalize()
{
}
