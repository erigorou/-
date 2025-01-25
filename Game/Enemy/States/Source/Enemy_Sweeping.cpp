#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Sound/Sound.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Enemy/States/Header/Enemy_Sweeping.h"


// ----------------------------------
// �R���X�g���N�^
// ----------------------------------
Enemy_Sweeping::Enemy_Sweeping(Enemy* enemy)
	: m_angle(0.f)
	, m_enemy(enemy)
	, m_totalSeconds()
{
}


// ----------------------------------
// �f�X�g���N�^
// ----------------------------------
Enemy_Sweeping::~Enemy_Sweeping()
{
}


// ----------------------------------
// ����������
// ----------------------------------
void Enemy_Sweeping::Initialize()
{
}



// ----------------------------------
// ���O�X�V����
// ----------------------------------
void Enemy_Sweeping::PreUpdate()
{
	// �o�ߎ��Ԃ̏�����
	m_totalSeconds = 0.0f;
	m_angle = DirectX::XMConvertToDegrees(m_enemy->GetAngle());

	auto cudgel = m_enemy->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetSweeping());			// ���_�̃X�e�[�g��ύX

	// ��
	m_enemy->SetFace(m_enemy->GetFaceAttacking());
}


// ----------------------------------
// �X�V����
// ----------------------------------
void Enemy_Sweeping::Update(const float& elapsedTime)
{
	using namespace DirectX::SimpleMath;


	// �o�ߎ��Ԃ̌v�Z
	m_totalSeconds += elapsedTime;

	float t = 0.0f;						// �C�[�W���O�Ɏg�p����ϐ�
	static float targetAngle = 0.0f;	// �ڕW�̊p�x

	if (m_totalSeconds <= CHARGE_TIME)	// �J�n���炽�߃��[�V�������Ȃ�
	{
		t = m_totalSeconds / CHARGE_TIME;
		targetAngle = m_angle - ROTATE_ANGLE * Easing::easeOutCirc(t);					// ���߃��[�V�������s��
	}

	else if (
		m_totalSeconds >= WINDUP_TIME &&	// �ҋ@���Ԃ��߂����@����
		m_totalSeconds <= ATTACK_TIME)		// �U�����Ԓ��Ȃ�
	{
		t = (m_totalSeconds - WINDUP_TIME) / (ATTACK_TIME - WINDUP_TIME);
		targetAngle = m_angle - ROTATE_ANGLE + ROTATE_ANGLE * Easing::easeOutBack(t);	// �ガ�������[�V�������s��
	}


	if(m_totalSeconds > END_TIME)	m_enemy->ChangeState(m_enemy->GetEnemyIdling());		// �ҋ@��ԂɑJ��


	m_enemy->SetAngle(DirectX::XMConvertToRadians(targetAngle));							// �p�x��ݒ�
}


// ----------------------------------
// ����X�V����
// ----------------------------------
void Enemy_Sweeping::PostUpdate()
{
	// ��̃X�e�[�g��ύX
	m_enemy->SetFace(m_enemy->GetFaceIdling());
}


// ----------------------------------
// �`�揈��
// ----------------------------------
void Enemy_Sweeping::CheckHitPlayerBody()
{

}


// ----------------------------------
// �I������
// ----------------------------------
void Enemy_Sweeping::Finalize()
{
}
