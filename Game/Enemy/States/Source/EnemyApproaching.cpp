#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Sound/Sound.h"
#include "Game/Messenger/EventMessenger.h"

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "Game/Enemy/States/Header/EnemyApproaching.h"


// --------------------------------------
// �R���X�g���N�^
// --------------------------------------
EnemyApproaching::EnemyApproaching(Enemy* enemy)
	:
	m_enemy(enemy),
	m_position{},
	m_velocity{},
	m_angle{},
	m_worldMat(DirectX::SimpleMath::Matrix::Identity),
	m_totalSeconds{}
{
	m_particles = m_enemy->GetPlayScene()->GetParticle();
}


// --------------------------------------
// �f�X�g���N�^
// --------------------------------------
EnemyApproaching::~EnemyApproaching()
{
}


// --------------------------------------
// ����������
// --------------------------------------
void EnemyApproaching::Initialize()
{
	// ���x��ݒ�i�O�ɂ��������Ȃ��j
	m_velocity = DirectX::SimpleMath::Vector3::Forward;
}


// --------------------------------------
// �X�e�[�g�ύX����(in)
// --------------------------------------
void EnemyApproaching::PreUpdate()
{
	// �o�ߎ��Ԃ�������
	m_totalSeconds = 0.0f; 
	// ���W�̎擾
	m_position	= m_enemy->GetPosition();
	// ��]�s��̎擾
	m_angle		= m_enemy->GetAngle();
	// ���[���h�s��̎擾
	m_worldMat	= m_enemy->GetWorldMatrix();
}


// --------------------------------------
// �X�V����
// --------------------------------------
void EnemyApproaching::Update(const float& elapsedTime)
{
	// ���v�̎��Ԃ��v�Z����
	m_totalSeconds += elapsedTime;

	// �T�C���g�̌v�Z(�㉺�ړ�)
	m_position.y = Math::CalculatingSinWave(m_totalSeconds, AMPLITUDE, FREQUENCY);
	// ��Βl����邱�ƂŃW�����v���Ă���悤�ȋ���������@��
	m_position.y = fabsf(m_position.y);
	// �v���C���[�̍��W���擾
	Vector3 playerPos = m_enemy->GetPlayScene()->GetPlayer()->GetPosition();
	// �ړ��ŃJ������h�炷
	if (m_position.y <= MINIMAL)
	{
		// �J������h�炷
		float shakePower = SHAKE_POWER;
		EventMessenger::Execute("CameraShake", &shakePower);
		// �p�[�e�B�N���𐶐�
		EventMessenger::Execute("CreateBashDust", &m_position);
		// �T�E���h���Đ�
		Sound::PlaySE(Sound::SE_TYPE::ENEMY_MOVE);

		// �v���C���[�Ƃ̋������߂��ꍇ
		if (DirectX::SimpleMath::Vector3::Distance(m_position, playerPos) <= MIN_DISTANCE)
		{
			// ���ʉ���炷
			Sound::PlaySE(Sound::SE_TYPE::ENEMY_MOVE);
			// �����_���Ɏ��͓x�̋��������邩���߂�
			int random = Math::RandomInt(0, 4);
			if (random <= 1)		m_enemy->ChangeState(m_enemy->GetEnemySweeping());	// �|��
			else if (random == 2)	m_enemy->ChangeState(m_enemy->GetEnemyAttacking());	// �U��
			else if (random == 3)	m_enemy->ChangeState(m_enemy->GetEnemyIdling());	// �������Ȃ�
			// �ȍ~�̏����͍s��Ȃ�
			return;
		}
	}

	// �G���猩���v���C���[�̈ʒu���v�Z����
	m_angle = Math::CalculationAngle(m_position, playerPos);
	// ��]�s��̍쐬
	Matrix angleMat = Matrix::CreateScale(Enemy::ENEMY_SCALE) * Matrix::CreateRotationY(-m_angle);
	// �O���Ɉړ�
	m_position += Vector3::Transform(m_velocity * elapsedTime * NORMALIZE_VELOCITY, angleMat);
	// ���b���o�߂őҋ@���[�V�����ɕύX
	if (m_totalSeconds >= TOTAL_TIME)
	{
		m_enemy->ChangeState(m_enemy->GetEnemyIdling());
	}

	// ��]�p��ݒ肷��
	m_enemy->SetAngle(m_angle);
	// ���W��ݒ肷��
	m_enemy->SetPosition(m_position);
}




// --------------------------------------
// �X�e�[�g�ύX����(out)
// --------------------------------------
void EnemyApproaching::PostUpdate()
{
	// ���[���h�s���S�̂ɐݒ肷��
	m_enemy->SetWorldMatrix(m_worldMat);
	// �G�̈ʒu��0�ŌŒ肷��
	m_position.y = 0.0f;
	// �G�̈ʒu��ݒ肷��
	m_enemy->SetPosition(m_position);
	// �J������h�炷
	float shakePower = SHAKE_POWER;
	EventMessenger::Execute("CameraShake", &shakePower);
	// �p�[�e�B�N���𐶐�
	EventMessenger::Execute("CreateBashDust", &m_position);
}


// --------------------------------------
// �I������
// --------------------------------------
void EnemyApproaching::Finalize()
{
}