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


// �R���X�g���N�^
EnemyApproaching::EnemyApproaching(Enemy* enemy)
	:
	m_enemy(enemy),
	m_position(0.0f, 0.0f, 0.0f),
	m_velocity(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f)* Enemy::ENEMY_SPEED),
	m_angle(0.0f),
	m_worldMat(DirectX::SimpleMath::Matrix::Identity),
	m_totalSeconds(0.0f),
	m_amplitude(1.0f),
	m_finishTime(1.0f),
	m_frequency(1.0f),
	m_shakePower(1.0f)
{
	m_particles = m_enemy->GetPlayScene()->GetParticle();
}


// �f�X�g���N�^
EnemyApproaching::~EnemyApproaching()
{
}


// ����������
void EnemyApproaching::Initialize()
{
	// ���x��ݒ�i�O�ɂ��������Ȃ��j
	m_velocity = DirectX::SimpleMath::Vector3::Forward;
}


// ���O�X�V����
void EnemyApproaching::PreUpdate()
{
	// �o�ߎ��Ԃ�������
	m_totalSeconds = 0.f; 

	m_position	= m_enemy->GetPosition();		// ���W�̎擾
	m_angle		= m_enemy->GetAngle();			// ��]�̎擾
	m_worldMat	= m_enemy->GetWorldMatrix();	// ���[���h�s��̎擾
}


// �X�V����
void EnemyApproaching::Update(const float& elapsedTime)
{

	using namespace DirectX::SimpleMath;
	
	// ���v�̎��Ԃ��v�Z����
	m_totalSeconds += elapsedTime;

	// �T�C���g�̌v�Z(�㉺�ړ�)
	m_position.y = Math::CalculatingSinWave(m_totalSeconds, m_amplitude, m_frequency);

	// ��Βl����邱�ƂŃW�����v���Ă���悤�ȋ���������@��
	m_position.y = fabsf(m_position.y);

	// �v���C���[�̍��W���擾
	Vector3 playerPos = m_enemy->GetPlayScene()->GetPlayer()->GetPosition();

	// �ړ��ŃJ������h�炷
	if (m_position.y <= MINIMAL)
	{
		// �J������h�炷
		EventMessenger::Execute("CameraShake", &m_shakePower);
		// �T�E���h���Đ�
		Sound::PlaySE(Sound::SE_TYPE::ENEMY_MOVE);


		// �v���C���[�Ƃ̋������߂��ꍇ
		if (DirectX::SimpleMath::Vector3::Distance(m_position, playerPos) <= 20.0f)
		{
			// ���ʉ���炷
			Sound::PlaySE(Sound::SE_TYPE::ENEMY_MOVE);

			int random = Math::RandomInt(0, 4);

			if (random <= 1)
				m_enemy->ChangeState(m_enemy->GetEnemySweeping());		// �|��
			else if (random == 2)
				m_enemy->ChangeState(m_enemy->GetEnemyAttacking());		// �U��
			else if (random == 3)
				m_enemy->ChangeState(m_enemy->GetEnemyIdling());		// �������Ȃ�

			return;
		}
	}

	// �G���猩���v���C���[�̈ʒu���v�Z����
	m_angle = Math::CalculationAngle(m_position, playerPos);
	
	// ��]�s��̍쐬
	Matrix angleMat  = Matrix::CreateScale(Enemy::ENEMY_SCALE)
					*= Matrix::CreateRotationY(-m_angle);

	// �O���Ɉړ�
	m_position += Vector3::Transform(m_velocity * elapsedTime * NORMALIZE_VELOCITY, angleMat);

	// 2�b�o�߂őҋ@���[�V�����ɕύX
	if (m_totalSeconds >= 2.0f)
	{
		m_enemy->ChangeState(m_enemy->GetEnemyIdling());
	}


	// ��]�p��ݒ肷��
	m_enemy->SetAngle(m_angle);

	// ���W��ݒ肷��
	m_enemy->SetPosition(m_position);
}




// ����X�V����
void EnemyApproaching::PostUpdate()
{
	// ���[���h�s���S�̂ɐݒ肷��
	m_enemy->SetWorldMatrix(m_worldMat);
	// �G�̈ʒu��0�ŌŒ肷��
	m_position.y = 0.f;
	m_enemy->SetPosition(m_position);

	EventMessenger::Execute("CameraShake", &m_shakePower);
	EventMessenger::Execute("CreateSlamDust", &m_position);
}


void EnemyApproaching::Finalize()
{
}