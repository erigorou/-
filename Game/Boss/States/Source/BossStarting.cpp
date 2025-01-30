#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/Sound/Sound.h"
#include "Game/Boss/States/Header/BossStarting.h"
// �֐����g�p����̂ɕK�v�Ȉ���
#include "Game/Boss/Boss.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"


// --------------------------
// �R���X�g���N�^
// --------------------------
BossStarting::BossStarting(Boss* Boss)
	:
	m_boss(Boss),
	m_angle{},
	m_totalSeconds{},
	m_isEndDelay(false)
{
}

// --------------------------
// �f�X�g���N�^
// --------------------------
BossStarting::~BossStarting()
{
}

// --------------------------
// ����������
// --------------------------
void BossStarting::Initialize()
{
}


// --------------------------
// ���O�X�V����
// --------------------------
void BossStarting::PreUpdate()
{
	// �o�ߎ��Ԃ�������
	m_totalSeconds = 0.0f;
	// ����̃X�e�[�g��ύX
	auto cudgel = m_boss->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());
	// ��̃X�e�[�g��ύX
	m_boss->SetFace(m_boss->GetFaceIdling());
}


// --------------------------
// �X�V����
// --------------------------
void BossStarting::Update(const float& elapsedTime)
{
	m_totalSeconds += elapsedTime;

	Vector3 playerPos = m_boss->GetPlayScene()->GetPlayer()->GetPosition();
	Vector3 parentPos = m_boss->GetPosition();

	// �G���猩���v���C���[�̈ʒu���v�Z����
	m_angle = Math::CalculationAngle(parentPos, playerPos);
	m_boss->SetAngle(m_angle);

	// �A�j���[�V�����X�V����
	UpdateAnimation();

	// �P�b�ōs����ύX����
	if (m_totalSeconds >= TOTAL_TIME)
	{
		// �G��������
		m_boss->ChangeState(m_boss->GetBossIdling());
	}
}



// --------------------------
// �A�j���[�V�����X�V����
// --------------------------
void BossStarting::UpdateAnimation()
{
	if (m_totalSeconds < DELAY_TIME)		UpdateDelay();
	else if (m_totalSeconds < MOVE_TIME)	UpdateMove();
}


// --------------------------
// �A�j���[�V�����̒x������
// --------------------------
void BossStarting::UpdateDelay()
{
	m_position = m_boss->GetPosition();
	m_boss->SetPosition(DirectX::SimpleMath::Vector3(m_position.x, START_HEIGHT, m_position.z));
}


// --------------------------
// �A�j���[�V�����̈ړ�����
// --------------------------
void BossStarting::UpdateMove()
{
	// �b���𐳋K��
	float t = (m_totalSeconds - DELAY_TIME) / (MOVE_TIME - DELAY_TIME);
	// �����Ƃ���ɏ����ʒu��ݒ肵�A���X�ɉ��ɍ~�낷
	m_position.y = std::fabsf(START_HEIGHT - (START_HEIGHT * Easing::easeInOutBack(t)));
	// �ʒu��ݒ�
	m_boss->SetPosition(m_position);

	// ���ȉ��̍����ɂȂ�����G�t�F�N�g�𔭓�
	if (m_position.y < PLAY_EFFECT_HEIGHT)
	{
		PlayEffect();
	}
}


// --------------------------
// �G�t�F�N�g���Đ�����
// --------------------------
void BossStarting::PlayEffect()
{
	// ��x�������s����
	if (m_isEndDelay) return;

	// �G�t�F�N�g���Đ�
	EventMessenger::Execute("CreateBashDust", &m_position);
	// �J������h�炷
	float shakePower = SHAKE_POWER;
	EventMessenger::Execute("CameraShake", &shakePower);
	// �������Đ�
	Sound::PlaySE(Sound::SE_TYPE::BOSS_MOVE);
	// �t���O��L���ɂ���
	m_isEndDelay = true;
}


// --------------------------
// ����X�V����
// --------------------------
void BossStarting::PostUpdate()
{
}


// --------------------------
// �I������
// --------------------------
void BossStarting::Finalize()
{
}