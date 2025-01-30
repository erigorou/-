#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

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
}


// --------------------------
// �ݒ菈��(in)
// --------------------------
void BossIdling::PreUpdate()
{
	// �o�ߎ��Ԃ�������
	m_totalSeconds = 0.0f;
	// ����̃X�e�[�g��ύX
	auto cudgel = m_boss->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());

	// ��̃X�e�[�g��ύX
	m_boss->SetFace(m_boss->GetFaceIdling());

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
	Vector3 playerPos = m_boss->GetPlayScene()->GetPlayer()->GetPosition();
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
	m_angle += angleDiff * 0.1f;
	// �p�x��ݒ�
	m_boss->SetAngle(m_angle);
}



// ----------------------------------
// ���̃X�e�[�g�ɑJ�ڂ��邩�����m
// ----------------------------------
void BossIdling::CheckNextState()
{
	// �v���C���[�̍��W���擾
	Vector3 playerPos = m_boss->GetPlayScene()->GetPlayer()->GetPosition();
	Vector3 parentPos = m_boss->GetPosition();

	// �P�b�ōs����ύX����
	if (m_totalSeconds >= TOTAL_TIME)
	{
		int random = Math::RandomInt(0, TOTAL_RATE);
		float distance = Vector3::Distance(parentPos, playerPos);
		// ���������̏ꍇ
		if (distance > FAR_DISTANCE)
		{
			if (random % 2 == 0)	m_boss->ChangeState(m_boss->GetBossApproaching());	// �Ǐ]
			else					m_boss->ChangeState(m_boss->GetBossDashAttacking());	// �_�b�V���U��
		}
		// �߂������̏ꍇ
		else
		{
			// �����_���ōs����ύX����
			if (random <= SWEEPING_RATE)
			{
				// �ガ����
				m_boss->ChangeState(m_boss->GetBossSweeping());
			}
			else if (random <= ATTACKING_RATE)
			{
				// �U��
				m_boss->ChangeState(m_boss->GetBossAttacking());
			}
			else if (random <= DASH_ATTACK_RATE)
			{
				// �_�b�V���U��
				m_boss->ChangeState(m_boss->GetBossDashAttacking());
			}
			else if (random == IDLING_RATE)
			{
				// �������Ȃ�
				m_boss->ChangeState(m_boss->GetBossIdling());
			}
		}
	}
}



// --------------------------
// �ݒ菈��(out)
// --------------------------
void BossIdling::PostUpdate()
{
	// �C���_������΂����ɋL��
}


// ---------------------------
// �I������
// ---------------------------
void BossIdling::Finalize()
{
}