#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries//MyLib/EasingFunctions.h"

// �֐����g�p����̂ɕK�v�Ȉ���
#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "Game/EnemyManager/EnemyManager.h"
#include "Game/Messenger/EventMessenger.h"

// �w�b�_�[�t�@�C��
#include "Game/Enemy/States/Header/EnemyDead.h"


// ---------------------------
// �R���X�g���N�^
// ---------------------------
EnemyDead::EnemyDead(Enemy* enemy)
	:
	m_angle(0.0f),
	m_enemy(enemy),
	m_totalSeconds(),
	m_tiltAngle(0.0f)
{
}


// ---------------------------
// �f�X�g���N�^
// ---------------------------
EnemyDead::~EnemyDead()
{
	Finalize();
}


// ---------------------------
// ����������
// ---------------------------
void EnemyDead::Initialize()
{
}


// ---------------------------
// �ύX����(in)
// ---------------------------
void EnemyDead::PreUpdate()
{
	// �o�ߎ��Ԃ�������
	m_totalSeconds = 0;

	// ��]���擾
	m_angle = m_enemy->GetAngle();
	// ��ԊJ�n���̌X�����擾
	m_startTilt = m_enemy->GetBodyTilt();

	// ����̃X�e�[�g��ύX
	auto cudgel = m_enemy->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());

	// ��̃X�e�[�g��ύX
	m_enemy->SetFace(m_enemy->GetFaceIdling());

	// �S�Ă̓G��HP��0�ɂ���
	m_enemy->GetPlayScene()->GetEnemyManager()->AllGoblinHPZero();
}


// ---------------------------
// �X�V����
// ---------------------------
void EnemyDead::Update(const float& elapsedTime)
{
	// �o�ߎ��Ԃ����Z
	m_totalSeconds += elapsedTime;

	// �P�b�ōs����ύX����
	if (m_totalSeconds >= TOTAL_TIME)
	{
		// �G��������
		m_enemy->DeadAction();
		return;
	}

	// �A�j���[�V�����̍X�V
	UpdateAnimation();
}


// ---------------------------
// �A�j���[�V�����̍X�V
// ---------------------------
void EnemyDead::UpdateAnimation()
{
	// ���K���������Ԃ����߂�
	float t = m_totalSeconds / TOTAL_TIME;

	// �C�[�W���O�A�j���[�V������p���ČX�������߂�
	m_tiltAngle = m_startTilt + ( MAX_TILT_ANGLE - m_startTilt) * Easing::easeOutBounce(t);

	// �X����ݒ�
	m_enemy->SetBodyTilt(DirectX::XMConvertToRadians(m_tiltAngle));

	// �J������h�炷�^�C�~���O��}��
	if (m_tiltAngle <= CAMERA_SHAKE_TIMING)
	{
		float shakePower = CAMERA_SHAKE_POWER;
		EventMessenger::Execute("CameraShake", &shakePower);

		DirectX::SimpleMath::Vector3 enemyPos = m_enemy->GetPosition();

		EventMessenger::Execute("CreateSpamDust", &enemyPos);
	}
}


// ---------------------------
// �ύX����(out)
// ---------------------------
void EnemyDead::PostUpdate()
{
}


// ---------------------------
// �I������
// ---------------------------
void EnemyDead::Finalize()
{
}