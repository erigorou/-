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
	m_totalSeconds = 0;
	// ����̃X�e�[�g��ύX
	auto cudgel = m_boss->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());

	// ��̃X�e�[�g��ύX
	m_boss->SetFace(m_boss->GetFaceIdling());
}


// --------------------------
// �X�V����
// --------------------------
void BossIdling::Update(const float& elapsedTime)
{
	using namespace DirectX::SimpleMath;
	m_totalSeconds += elapsedTime;

	Vector3 playerPos = m_boss->GetPlayScene()->GetPlayer()->GetPosition();
	Vector3 parentPos = m_boss->GetPosition();

	// �G���猩���v���C���[�̈ʒu���v�Z����
	m_angle = Math::CalculationAngle(parentPos, playerPos);
	m_boss->SetAngle(m_angle);

	// �P�b�ōs����ύX����
	if (m_totalSeconds >= 1.0f)
	{
		int random = Math::RandomInt(0, 10);
		float distance = Vector3::Distance(parentPos, playerPos);
		// ���������̏ꍇ
		if (distance > 20.0f)
		{
			if (random % 2 == 0)
			{
				m_boss->ChangeState(m_boss->GetBossApproaching());	// �Ǐ]
			}
			else
			{
				m_boss->ChangeState(m_boss->GetBossDashAttacking());	// �_�b�V���U��
			}
		}
		// �߂������̏ꍇ
		else
		{
			if (random <= 3)
				m_boss->ChangeState(m_boss->GetBossSweeping());		// �|��
			else if(random <= 7)
				m_boss->ChangeState(m_boss->GetBossAttacking());		// �U��
			else if (random <= 8)
				m_boss->ChangeState(m_boss->GetBossDashAttacking());	// �_�b�V���U��
			else if(random == 10)
				m_boss->ChangeState(m_boss->GetBossIdling());		// �������Ȃ�
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