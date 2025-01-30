#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Sound/Sound.h"

#include "Game/Boss/Boss.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Boss/States/Header/BossSweeping.h"


// ----------------------------------
// �R���X�g���N�^
// ----------------------------------
BossSweeping::BossSweeping(Boss* Boss)
	: 
	m_angle{},
	m_boss(Boss),
	m_totalSeconds{}
{
}


// ----------------------------------
// �f�X�g���N�^
// ----------------------------------
BossSweeping::~BossSweeping()
{
}


// ----------------------------------
// ����������
// ----------------------------------
void BossSweeping::Initialize()
{
}



// ----------------------------------
// ���O�X�V����
// ----------------------------------
void BossSweeping::PreUpdate()
{
	// �o�ߎ��Ԃ̏�����
	m_totalSeconds = 0.0f;
	m_angle = DirectX::XMConvertToDegrees(m_boss->GetAngle());

	auto cudgel = m_boss->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetSweeping());			// ���_�̃X�e�[�g��ύX

	// ��
	m_boss->SetFace(m_boss->GetFaceAttacking());
}


// ----------------------------------
// �X�V����
// ----------------------------------
void BossSweeping::Update(const float& elapsedTime)
{
	using namespace DirectX::SimpleMath;


	// �o�ߎ��Ԃ̌v�Z
	m_totalSeconds += elapsedTime;

	float t = 0.0f;						// �C�[�W���O�Ɏg�p����ϐ�
	static float targetAngle = 0.0f;	// �ڕW�̊p�x

	if (m_totalSeconds <= CHARGE_TIME)	// �J�n���炽�߃��[�V�������Ȃ�
	{
		t = m_totalSeconds / CHARGE_TIME;
		targetAngle = m_angle - ROTATE_ANGLE * Easing::easeOutCirc(t);	// ���߃��[�V�������s��
	}

	else if (
		m_totalSeconds >= WINDUP_TIME &&	// �ҋ@���Ԃ��߂����@����
		m_totalSeconds <= ATTACK_TIME)		// �U�����Ԓ��Ȃ�
	{
		t = (m_totalSeconds - WINDUP_TIME) / (ATTACK_TIME - WINDUP_TIME);
		targetAngle = m_angle - ROTATE_ANGLE + ROTATE_ANGLE * Easing::easeOutBack(t);	// �ガ�������[�V�������s��
	}


	if(m_totalSeconds > END_TIME)	m_boss->ChangeState(m_boss->GetBossIdling());		// �ҋ@��ԂɑJ��


	m_boss->SetAngle(DirectX::XMConvertToRadians(targetAngle));							// �p�x��ݒ�
}


// ----------------------------------
// ����X�V����
// ----------------------------------
void BossSweeping::PostUpdate()
{
	// ��̃X�e�[�g��ύX
	m_boss->SetFace(m_boss->GetFaceIdling());
}



// ----------------------------------
// �I������
// ----------------------------------
void BossSweeping::Finalize()
{
}
