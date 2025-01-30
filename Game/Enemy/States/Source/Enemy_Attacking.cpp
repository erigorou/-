#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "Game/Enemy/States/Header/Enemy_Attacking.h"

// --------------------------------------
// �R���X�g���N�^
// --------------------------------------
Enemy_Attacking::Enemy_Attacking(Enemy* enemy)
	:
	m_angle{},
	m_enemy(enemy),
	m_totalSeconds{}
{
}

// --------------------------------------
// �f�X�g���N�^
// --------------------------------------
Enemy_Attacking::~Enemy_Attacking()
{
}


// --------------------------------------
// ����������
// --------------------------------------
void Enemy_Attacking::Initialize()
{
}


// --------------------------------------
// �X�e�[�g�X�V�����iin�j
// --------------------------------------
void Enemy_Attacking::PreUpdate()
{
	// �o�ߎ��Ԃ�������
	m_totalSeconds = 0.0f;
	// ����̃X�e�[�g��ύX
	auto cudgel = m_enemy->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetAttacking());

	// ��̃X�e�[�g��ύX
	m_enemy->SetFace(m_enemy->GetFaceAttacking());
}


// --------------------------------------
// �X�V����
// --------------------------------------
void Enemy_Attacking::Update(const float& elapsedTime)
{
	// �o�ߎ��Ԃ����Z
	m_totalSeconds += elapsedTime;

	// �v���C���[�̍��W���擾
	DirectX::SimpleMath::Vector3 playerPos = m_enemy->GetPlayScene()->GetPlayer()->GetPosition();
	// �G�̍��W���擾
	DirectX::SimpleMath::Vector3 parentPos = m_enemy->GetPosition();
	// �G���猩���v���C���[�̈ʒu���v�Z����
	m_angle = Math::CalculationAngle(parentPos, playerPos);

	// �U�����܂ł̓v���C���[��ǔ�����悤�ɂ���
	if (m_totalSeconds <= Cudgel_Attacking::ATTACK_TIME)
	{
		// �v���C���[��ǔ�
		m_enemy->SetAngle(m_angle);
	}

	if (m_totalSeconds >= TOTAL_TIME)
	{
		m_enemy->ChangeState(m_enemy->GetEnemyIdling());	// �ҋ@��ԂɑJ��
	}
}


// --------------------------------------
// �X�e�[�g�ύX����(out)
// --------------------------------------
void Enemy_Attacking::PostUpdate()
{
	// ����̃X�e�[�g��ύX����
	auto cudgel = m_enemy->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());

	// ��̃X�e�[�g��ύX
	m_enemy->SetFace(m_enemy->GetFaceIdling());
}


// --------------------------------------
// �I������
// --------------------------------------
void Enemy_Attacking::Finalize()
{
}