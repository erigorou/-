#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/Weapon/WeaponState.h"

// �֐����g�p����̂ɕK�v�Ȉ���
#include "Game/Boss/Boss.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"

// �w�b�_�[�t�@�C��
#include "Game/Boss/States/Header/BossIdling.h"

// --------------------------
// �R���X�g���N�^
// --------------------------
BossIdling::BossIdling(Boss* boss)
	:
	m_boss(boss),
	m_angle{},
	m_totalSeconds{}
{
}


// --------------------------
// �f�X�g���N�^
// --------------------------
BossIdling::~BossIdling()
{
}


// --------------------------
// ����������
// --------------------------
void BossIdling::Initialize()
{
	// �v���C���[�̎擾
	void* object = EventMessenger::ExecuteGetter("GetPlayerObject");
	m_player = object ? static_cast<IObject*>(object) : nullptr;
}


// --------------------------
// �ݒ菈��(in)
// --------------------------
void BossIdling::PreUpdate()
{
	// �o�ߎ��Ԃ�������
	m_totalSeconds = 0.0f;

	// ����̃X�e�[�g��ύX
	CudgelState state = CudgelState::Idle;
	EventMessenger::Execute("ChangeCudgelState", &state);

	// ���ύX
	FaceState face = FaceState::Idling;
	EventMessenger::Execute("ChangeBossFace", &face);

	// ��]���擾
	m_angle = m_boss->GetAngle();
}


// --------------------------
// �X�V����
// --------------------------
void BossIdling::Update(const float& elapsedTime)
{
	// �o�ߎ��Ԃ����Z
	m_totalSeconds += elapsedTime;
	// �A�j���[�V�����̍X�V
	UpdateAnimation();
	// ���̃X�e�[�g�ɑJ�ڂ��邩�����m
	CheckNextState();
}


// --------------------------
// �G�̋����X�V����
// --------------------------
void BossIdling::UpdateAnimation()
{
	// �v���C���[�̍��W���擾
	Vector3 playerPos = m_player->GetPosition();
	Vector3 parentPos = m_boss->GetPosition();

	// �A���O�����v�Z
	float targetAngle = Math::CalculationAngle(parentPos, playerPos);
	// �p�x�����v�Z
	float angleDiff = targetAngle - m_angle;

	// �p�x����180�x�𒴂���ꍇ�A��]�����𔽓]
	if (angleDiff > DirectX::XM_PI)
	{
		angleDiff -= DirectX::XM_2PI;
	}
	else if (angleDiff < -DirectX::XM_PI)
	{
		angleDiff += DirectX::XM_2PI;
	}
	// ���`��ԂōŒZ�����ŉ�]
	m_angle += angleDiff * LERP_RATE;
	// �p�x��ݒ�
	m_boss->SetAngle(m_angle);
}



// ----------------------------------
// ���̃X�e�[�g�ɑJ�ڂ��邩�����m
// ----------------------------------
void BossIdling::CheckNextState()
{
	// �v���C���[�̍��W���擾
	Vector3 playerPos = m_player->GetPosition();
	Vector3 parentPos = m_boss->GetPosition();

	// �P�b�ōs����ύX����
	if (m_totalSeconds >= TOTAL_TIME)
	{
		// �����_���ōs����ύX����
		int random = Math::RandomInt(0, TOTAL_RATE);
		// �v���C���[�Ƃ̋������v�Z
		float distance = Vector3::Distance(parentPos, playerPos);
		// ���̃{�X�̃X�e�[�g
		BossState state = BossState::Idling;

		// ���������̏ꍇ
		if (distance > FAR_DISTANCE)
		{
			// �Ǐ]
			if (random % 2 == 0) state = BossState::Approaching;
			// �_�b�V���U��
			else state = BossState::DashAttacking;
		}
		// �߂������̏ꍇ
		else
		{
			// �ガ����
			if (random <= SWEEPING_RATE) state = BossState::Sweeping;
			// �U��
			else if (random <= ATTACKING_RATE) state = BossState::Attacking;
			// �_�b�V���U��
			else if (random <= DASH_ATTACK_RATE) state = BossState::DashAttacking;
			// �������Ȃ�
			else if (random <= IDLING_RATE) state = BossState::Idling;
		}

		// �{�X�̃X�e�[�g��ύX
		EventMessenger::Execute("ChangeBossState", &state);
	}
}



// --------------------------
// �ݒ菈��(out)
// --------------------------
void BossIdling::PostUpdate()
{
}


// ---------------------------
// �I������
// ---------------------------
void BossIdling::Finalize()
{
}