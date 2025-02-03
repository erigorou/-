#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Sound/Sound.h"
#include "Game/Messenger/EventMessenger.h"

#include "Game/Player/Player.h"
#include "Game/Boss/Boss.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "Game/Boss/States/Header/BossApproaching.h"


// --------------------------------------
// �R���X�g���N�^
// --------------------------------------
BossApproaching::BossApproaching(Boss* boss)
	:
	m_boss(boss),
	m_position{},
	m_velocity{},
	m_angle{},
	m_totalSeconds{}
{
}


// --------------------------------------
// �f�X�g���N�^
// --------------------------------------
BossApproaching::~BossApproaching()
{
}


// --------------------------------------
// ����������
// --------------------------------------
void BossApproaching::Initialize()
{
	// ���x��ݒ�i�O�ɂ��������Ȃ��j
	m_velocity = DirectX::SimpleMath::Vector3::Forward;	

	// �v���C���[�̎擾
	void* player = EventMessenger::ExecuteGetter(GetterList::GetPlayer);
	m_player = player ? static_cast<IObject*>(player) : nullptr;
}


// --------------------------------------
// �X�e�[�g�ύX����(in)
// --------------------------------------
void BossApproaching::PreUpdate()
{
	// �o�ߎ��Ԃ�������
	m_totalSeconds = 0.0f; 
	// ���W�̎擾
	m_position	= m_boss->GetPosition();
	// ��]�s��̎擾
	m_angle		= m_boss->GetAngle();
}


// --------------------------------------
// �X�V����
// --------------------------------------
void BossApproaching::Update(const float& elapsedTime)
{
	// ���v�̎��Ԃ��v�Z����
	m_totalSeconds += elapsedTime;
	// �A�j���[�V�����̍X�V
	UpdateAnimation(elapsedTime);
	// ���̃X�e�[�g�Ɉڍs���邩�����m
	CheckNextState();
}



// --------------------------------------
// �A�j���[�V�����̍X�V
// --------------------------------------
void BossApproaching::UpdateAnimation(float elapsedTime)
{
	// �T�C���g�̌v�Z(�㉺�ړ�)
	m_position.y = Math::CalculatingSinWave(m_totalSeconds, AMPLITUDE, FREQUENCY);
	// ��Βl����邱�ƂŃW�����v���Ă���悤�ȋ���������@��
	m_position.y = fabsf(m_position.y);

	// �v���C���[�̍��W���擾
	Vector3 playerPos = m_player->GetPosition();

	// �G���猩���v���C���[�̈ʒu���v�Z����
	m_angle = Math::CalculationAngle(m_position, playerPos);
	// ��]�s��̍쐬
	Matrix angleMat = Matrix::CreateScale(Boss::BOSS_SCALE) * Matrix::CreateRotationY(-m_angle);
	// �����Ă�������̑O���Ɉړ�
	m_position += Vector3::Transform(m_velocity * elapsedTime * NORMALIZE_VELOCITY, angleMat);
	// ��]�p��ݒ肷��
	m_boss->SetAngle(m_angle);
	// ���W��ݒ肷��
	m_boss->SetPosition(m_position);
}



// --------------------------------------
// �����̃X�e�[�g�Ɉڍs���邩�����m
// --------------------------------------
void BossApproaching::CheckNextState()
{	
	// �v���C���[�̍��W���擾
	Vector3 playerPos = m_player->GetPosition();

	// �ړ��ŃJ������h�炷
	if (m_position.y <= MINIMAL)
	{
		// �J������h�炷
		float shakePower = SHAKE_POWER;
		EventMessenger::Execute(EventList::ShakeCamera, &shakePower);
		// �p�[�e�B�N���𐶐�
		EventMessenger::Execute(EventList::CreateBashDust, &m_position);
		// �T�E���h���Đ�
		Sound::PlaySE(Sound::SE_TYPE::BOSS_MOVE);

		// �v���C���[�Ƃ̋������߂��ꍇ
		if (DirectX::SimpleMath::Vector3::Distance(m_position, playerPos) <= MIN_DISTANCE)
		{
			// �{�X�̏�Ԃ�ۑ�����ϐ�
			BossState state = BossState::Idling;

			// �����_���Ɏ��͓x�̋��������邩���߂�
			int random = Math::RandomInt(0, 4);
			if (random <= 1)		state = BossState::Sweeping;	// �ガ����
			else if (random == 2)	state = BossState::Attacking;	// �U��
			else if (random == 3)	state = BossState::Idling;		// �������Ȃ�

			// ��Ԃ�ύX
			EventMessenger::Execute(EventList::ChangeBossState, &state);

			// �ȍ~�̏����͍s��Ȃ�
			return;
		}
	}

	// ���b���o�߂őҋ@���[�V�����ɕύX
	if (m_totalSeconds >= TOTAL_TIME)
	{
		// �ҋ@��ԂɕύX
		BossState state = BossState::Idling;
		EventMessenger::Execute(EventList::ChangeBossState, &state);
	}
}


// --------------------------------------
// �X�e�[�g�ύX����(out)
// --------------------------------------
void BossApproaching::PostUpdate()
{
	// �G�̈ʒu��0�ŌŒ肷��
	m_position.y = 0.0f;
	// �G�̈ʒu��ݒ肷��
	m_boss->SetPosition(m_position);
	// �J������h�炷
	float shakePower = SHAKE_POWER;
	EventMessenger::Execute(EventList::ShakeCamera, &shakePower);
	// �p�[�e�B�N���𐶐�
	EventMessenger::Execute(EventList::CreateBashDust, &m_position);
}


// --------------------------------------
// �I������
// --------------------------------------
void BossApproaching::Finalize()
{
}