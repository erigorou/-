#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

// �֐����g�p����̂ɕK�v�Ȉ���
#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"

// �w�b�_�[�t�@�C��
#include "Game/Enemy/States/Header/EnemyIdling.h"

// --------------------------
// �R���X�g���N�^
// --------------------------
EnemyIdling::EnemyIdling(Enemy* enemy)
	:
	m_angle(0.f),
	m_enemy(enemy),
	m_totalSeconds()
{
}


// --------------------------
// �f�X�g���N�^
// --------------------------
EnemyIdling::~EnemyIdling()
{
}


// --------------------------
// ����������
// --------------------------
void EnemyIdling::Initialize()
{
}


// --------------------------
// �ݒ菈��(in)
// --------------------------
void EnemyIdling::PreUpdate()
{
	// �o�ߎ��Ԃ�������
	m_totalSeconds = 0;
	// ����̃X�e�[�g��ύX
	auto cudgel = m_enemy->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());

	// ��̃X�e�[�g��ύX
	m_enemy->SetFace(m_enemy->GetFaceIdling());
}


// --------------------------
// �X�V����
// --------------------------
void EnemyIdling::Update(const float& elapsedTime)
{
	using namespace DirectX::SimpleMath;
	m_totalSeconds += elapsedTime;

	Vector3 playerPos = m_enemy->GetPlayScene()->GetPlayer()->GetPosition();
	Vector3 parentPos = m_enemy->GetPosition();

	// �G���猩���v���C���[�̈ʒu���v�Z����
	m_angle = Math::CalculationAngle(parentPos, playerPos);
	m_enemy->SetAngle(m_angle);

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
				m_enemy->ChangeState(m_enemy->GetEnemyApproaching());	// �Ǐ]
			}
			else
			{
				m_enemy->ChangeState(m_enemy->GetEnemyDashAttacking());	// �_�b�V���U��
			}
		}
		// �߂������̏ꍇ
		else
		{
			if (random <= 3)
				m_enemy->ChangeState(m_enemy->GetEnemySweeping());		// �|��
			else if(random <= 7)
				m_enemy->ChangeState(m_enemy->GetEnemyAttacking());		// �U��
			else if (random <= 8)
				m_enemy->ChangeState(m_enemy->GetEnemyDashAttacking());	// �_�b�V���U��
			else if(random == 10)
				m_enemy->ChangeState(m_enemy->GetEnemyIdling());		// �������Ȃ�
		}
	}
}

// --------------------------
// �ݒ菈��(out)
// --------------------------
void EnemyIdling::PostUpdate()
{
	// �C���_������΂����ɋL��
}


// ---------------------------
// �I������
// ---------------------------
void EnemyIdling::Finalize()
{
}