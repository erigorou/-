#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/Sound/Sound.h"
// �֐����g�p����̂ɕK�v�Ȉ���
#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"

// �w�b�_�[�t�@�C��
#include "Game/Enemy/States/Header/EnemyStarting.h"


// �R���X�g���N�^
EnemyStarting::EnemyStarting(Enemy* enemy)
	:
	m_angle(0.f),
	m_enemy(enemy),
	m_totalSeconds(),
	m_isEndDelay(false)
{
}


// �f�X�g���N�^
EnemyStarting::~EnemyStarting()
{
}


// ����������
void EnemyStarting::Initialize()
{
}


// ���O�X�V����
void EnemyStarting::PreUpdate()
{
	// �o�ߎ��Ԃ�������
	m_totalSeconds = 0;
	// ����̃X�e�[�g��ύX
	auto cudgel = m_enemy->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());

	// ��̃X�e�[�g��ύX
	m_enemy->SetFace(m_enemy->GetFaceIdling());
}


// �X�V����
void EnemyStarting::Update(const float& elapsedTime)
{

	using namespace DirectX::SimpleMath;
	m_totalSeconds += elapsedTime;

	Vector3 playerPos = m_enemy->GetPlayScene()->GetPlayer()->GetPosition();
	Vector3 parentPos = m_enemy->GetPosition();

	// �G���猩���v���C���[�̈ʒu���v�Z����
	m_angle = Math::CalculationAngle(parentPos, playerPos);
	m_enemy->SetAngle(m_angle);

	UpdateAnimation();

	// �P�b�ōs����ύX����
	if (m_totalSeconds >= TOTAL_TIME)
	{
		int random = Math::RandomInt(0, 10);
		float distance = Vector3::Distance(parentPos, playerPos);
		// ���������̏ꍇ
		if (distance > 20.0f)
		{
			if (random % 2 == 0)	m_enemy->ChangeState(m_enemy->GetEnemyApproaching());	// �Ǐ]
			else					m_enemy->ChangeState(m_enemy->GetEnemyDashAttacking());	// �_�b�V���U��
		}

		// �߂������̏ꍇ
		else
		{
			if (random <= 3)		m_enemy->ChangeState(m_enemy->GetEnemySweeping());		// �|��
			else if(random <= 7)	m_enemy->ChangeState(m_enemy->GetEnemyAttacking());		// �U��
			else if (random == 10)	m_enemy->ChangeState(m_enemy->GetEnemyDashAttacking());	// �_�b�V���U��
		}
	}
}


void EnemyStarting::UpdateAnimation()
{
	if (m_totalSeconds < DELAY_TIME)		UpdateDelay();
	else if (m_totalSeconds < MOVE_TIME)	UpdateMove();
}


void EnemyStarting::UpdateDelay()
{
	m_position = m_enemy->GetPosition();
	m_enemy->SetPosition(DirectX::SimpleMath::Vector3(m_position.x, START_HEIGHT, m_position.z));
}


void EnemyStarting::UpdateMove()
{
	// �b���𐳋K��
	float t = (m_totalSeconds - DELAY_TIME) / (MOVE_TIME - DELAY_TIME);
	// �����Ƃ���ɏ����ʒu��ݒ肵�A���X�ɉ��ɍ~�낷
	m_position.y = std::fabsf(START_HEIGHT - (START_HEIGHT * Easing::easeInOutBack(t)));
	// �ʒu��ݒ�
	m_enemy->SetPosition(m_position);

	// ���ȉ��̍����ɂȂ�����G�t�F�N�g�𔭓�
	if (m_position.y < PLAY_EFFECT_HEIGHT)
	{
		PlayEffect();
	}
}


void EnemyStarting::PlayEffect()
{
	// ��x�������s����
	if (m_isEndDelay) return;

	// �G�t�F�N�g�𔭓�
	m_enemy->GetPlayScene()->GetParticle()->CreateSlamDust(m_position);

	float shakePower = 2.0f;
	EventMessenger::Execute("CameraShake", &shakePower);


	// �������Đ�
	Sound::PlaySE(Sound::SE_TYPE::ENEMY_MOVE);

	// �t���O��L���ɂ���
	m_isEndDelay = true;
}





// ����X�V����
void EnemyStarting::PostUpdate()
{
	// �C���_������΂����ɋL��
}

void EnemyStarting::Finalize()
{
}