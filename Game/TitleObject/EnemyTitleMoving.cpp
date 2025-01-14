#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "TitleEnemy.h"
#include "EnemyTitleMoving.h"
#include "EnemyTitleIdling.h"


// �R���X�g���N�^
EnemyTitleMoving::EnemyTitleMoving(TitleEnemy* enemy)
	: m_enemy(enemy)
	, m_position(0.0f, 0.0f, 0.0f)
	, m_velocity(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f))
	, m_angle(0.0f)
	, m_worldMat(DirectX::SimpleMath::Matrix::Identity)
	, m_totalSeconds(0.0f)
	, m_amplitude(2.0f)
	, m_finishTime(1.0f)
	, m_frequency(1.0f)
	, m_isJump(false)
	, m_canShake(false)
{
}


// �f�X�g���N�^
EnemyTitleMoving::~EnemyTitleMoving()
{
}


// ����������
void EnemyTitleMoving::Initialize()
{
	// ���x��ݒ�i�O�ɂ��������Ȃ��j
	m_velocity = DirectX::SimpleMath::Vector3::Forward;
}


// ���O�X�V����
void EnemyTitleMoving::PreUpdate()
{
	using namespace DirectX::SimpleMath;

	// �o�ߎ��Ԃ�������
	m_totalSeconds = 0.0f; 

	m_position = Vector3::Zero;
}


// �X�V����
void EnemyTitleMoving::Update(const float& elapsedTime)
{	
	// ���v�̎��Ԃ��v�Z����
	m_totalSeconds += elapsedTime;

	// �T�C���g�̌v�Z(�㉺�ړ�)
	m_position.y = Math::CalculatingSinWave(m_totalSeconds, m_amplitude, m_frequency);

	// ��Βl����邱�ƂŃW�����v���Ă���悤�ȋ���������@��
	m_position.y = fabsf(m_position.y);

	// �ړ��ŃJ������h�炷
	if (m_position.y <= MINIMAL)
	{
		//m_enemy->GetTitleScene()->SetShakeCamera();
		m_enemy->ChangeState(m_enemy->GetTitleEnemyIdling());
	}

	// ��]�p��ݒ肷��
	m_enemy->SetAngle(m_angle);

	// ���W��ݒ肷��
	m_enemy->SetPosition(m_position);
}




// ����X�V����
void EnemyTitleMoving::PostUpdate()
{
	// ���[���h�s���S�̂ɐݒ肷��
	m_enemy->SetWorldMatrix(m_worldMat);
	// �G�̈ʒu��0�ŌŒ肷��
	m_position.y = 0.f;
	m_enemy->SetPosition(m_position);

	// �X�p��
	//m_enemy->GetTitleScene()->CleateSpamDust(m_position);
}


void EnemyTitleMoving::Finalize()
{
}