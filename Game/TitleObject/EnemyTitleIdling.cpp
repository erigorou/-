#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "TitleEnemy.h"
#include "EnemyTitleIdling.h"
#include "EnemyTitleMoving.h"

// �R���X�g���N�^
EnemyTitleIdling::EnemyTitleIdling(TitleEnemy* enemy)
	: m_enemy(enemy)
	, m_position(0.0f, 0.0f, 0.0f)
	, m_velocity(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f)* TitleEnemy::TITLE_ENEMY_SPEED)
	, m_model(nullptr)
	, m_angle(0.0f)
	, m_worldMat(DirectX::SimpleMath::Matrix::Identity)
	, m_totalSeconds(0.0f)
	, m_amplitude(1.0f)
	, m_finishTime(1.0f)
	, m_frequency(1.0f)
	, m_isJump(false)
	, m_canShake(false)
{
}


// �f�X�g���N�^
EnemyTitleIdling::~EnemyTitleIdling()
{
}


// ����������
void EnemyTitleIdling::Initialize(DirectX::Model* model)
{
	using namespace DirectX;
	// ���f�����擾
	m_model = model;
	// ���x��ݒ�i�O�ɂ��������Ȃ��j
	m_velocity = SimpleMath::Vector3::Forward;
}


// ���O�X�V����
void EnemyTitleIdling::PreUpdate()
{
	using namespace DirectX::SimpleMath;

	// �o�ߎ��Ԃ�������
	m_totalSeconds = 0.0f; 

	m_position = Vector3::Zero;
}


// �X�V����
void EnemyTitleIdling::Update(const float& elapsedTime)
{

	using namespace DirectX::SimpleMath;
	
	// ���v�̎��Ԃ��v�Z����
	m_totalSeconds += elapsedTime;


	if (m_totalSeconds > TitleEnemy::COOL_TIME)
	{
		m_enemy->ChangeState(m_enemy->GetTitleEnemyTitleMoving());
	}


	// ��]�s��̍쐬
	Matrix angleMat  = Matrix::CreateScale(TitleEnemy::TITLE_ENEMY_SCALE)
					*= Matrix::CreateRotationY(-m_angle);

	// ��]�p��ݒ肷��
	m_enemy->SetAngle(m_angle);

	// ���W��ݒ肷��
	m_enemy->SetPosition(m_position);
}




// ����X�V����
void EnemyTitleIdling::PostUpdate()
{
	// ���[���h�s���S�̂ɐݒ肷��
	m_enemy->SetWorldMatrix(m_worldMat);
	// �G�̈ʒu��0�ŌŒ肷��
	m_position.y = 0.f;
	m_enemy->SetPosition(m_position);
}


// �`�揈��
void EnemyTitleIdling::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
	)
{
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(states);
	UNREFERENCED_PARAMETER(view);
	UNREFERENCED_PARAMETER(projection);
	UNREFERENCED_PARAMETER(m_model);


#ifdef _DEBUG
	CommonResources* resources = CommonResources::GetInstance();
	auto debugString = resources->GetDebugString();

	UNREFERENCED_PARAMETER(debugString);
#endif // _DEBUG
}


void EnemyTitleIdling::Finalize()
{
}