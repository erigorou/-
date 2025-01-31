#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries//MyLib/EasingFunctions.h"

// �֐����g�p����̂ɕK�v�Ȉ���
#include "Game/Boss/Boss.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "Game/EnemyManager/EnemyManager.h"
#include "Game/Messenger/EventMessenger.h"

// �w�b�_�[�t�@�C��
#include "Game/Boss/States/Header/BossDead.h"


// ---------------------------
// �R���X�g���N�^
// ---------------------------
BossDead::BossDead(Boss* boss)
	: 
	m_boss(boss),
	m_angle{},
	m_totalSeconds{},
	m_tiltAngle{}
{
}


// ---------------------------
// �f�X�g���N�^
// ---------------------------
BossDead::~BossDead()
{
	Finalize();
}


// ---------------------------
// ����������
// ---------------------------
void BossDead::Initialize()
{
}


// ---------------------------
// �ύX����(in)
// ---------------------------
void BossDead::PreUpdate()
{
	// �o�ߎ��Ԃ�������
	m_totalSeconds = 0;
	// ��]���擾
	m_angle = m_boss->GetAngle();
	// ��ԊJ�n���̌X�����擾
	m_startTilt = m_boss->GetBodyTilt();
	// ����̃X�e�[�g��ύX
	auto cudgel = m_boss->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());
	// ��̃X�e�[�g��ύX
	m_boss->SetFace(m_boss->GetFaceIdling());


	// �S�Ă̓G��HP��0�ɂ���
	EventMessenger::Execute("DeleteAllGoblin", nullptr);
}


// ---------------------------
// �X�V����
// ---------------------------
void BossDead::Update(const float& elapsedTime)
{
	// �o�ߎ��Ԃ����Z
	m_totalSeconds += elapsedTime;

	// �P�b�ōs����ύX����
	if (m_totalSeconds >= TOTAL_TIME)
	{
		// �G��������
		m_boss->DeadAction();
		return;
	}
	// �A�j���[�V�����̍X�V
	UpdateAnimation();
}


// ---------------------------
// �A�j���[�V�����̍X�V
// ---------------------------
void BossDead::UpdateAnimation()
{
	// ���K���������Ԃ����߂�
	float t = m_totalSeconds / TOTAL_TIME;

	// �C�[�W���O�A�j���[�V������p���ČX�������߂�
	m_tiltAngle = m_startTilt + ( MAX_TILT_ANGLE - m_startTilt) * Easing::easeOutBounce(t);

	// �X����ݒ�
	m_boss->SetBodyTilt(DirectX::XMConvertToRadians(m_tiltAngle));

	// �J������h�炷�^�C�~���O��}��
	if (m_tiltAngle <= CAMERA_SHAKE_TIMING)
	{
		float shakePower = CAMERA_SHAKE_POWER;
		EventMessenger::Execute("CameraShake", &shakePower);

		DirectX::SimpleMath::Vector3 BossPos = m_boss->GetPosition();

		EventMessenger::Execute("CreateBashDust", &BossPos);
	}
}


// ---------------------------
// �ύX����(out)
// ---------------------------
void BossDead::PostUpdate()
{
}


// ---------------------------
// �I������
// ---------------------------
void BossDead::Finalize()
{
}