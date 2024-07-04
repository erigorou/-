#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/States/Header/EnemyApproaching.h"


// �R���X�g���N�^
EnemyApproaching::EnemyApproaching(Enemy* enemy)
	:
	m_enemy(enemy),
	m_position(0.f, 0.f, 0.f),
	m_velocity(DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f)* Enemy::ENEMY_SPEED),
	m_angle(0.f),
	m_worldMat(DirectX::SimpleMath::Matrix::Identity),
	m_totalSeconds(0.f),
	m_amplitude(1.f),
	m_finishTime(1.f),
	m_frequency(1.0f)
{
}


// �f�X�g���N�^
EnemyApproaching::~EnemyApproaching()
{
}


// ����������
void EnemyApproaching::Initialize(DirectX::Model* model)
{
	using namespace DirectX;
	// ���x��ݒ�i�O�ɂ��������Ȃ��j
	m_velocity = SimpleMath::Vector3::Forward;
	// �̂̓����蔻��̐���
	m_boundingSphereBody = BoundingSphere();
	// �̂̓����蔻��̃T�C�Y����W��ݒ�
	m_boundingSphereBody.Radius = Enemy::ENEMY_SCALE * 12.f;
}


// ���O�X�V����
void EnemyApproaching::PreUpdate()
{
	using namespace DirectX::SimpleMath;

	// �o�ߎ��Ԃ�������
	m_totalSeconds = 0.f; 
	// �G�̍��W���擾
	m_position = m_enemy->GetPosition();
	// �����̉�]�p��ݒ�
	m_angle = m_enemy->GetAngle();
	// ���[���h�s��̐ݒ�
	m_worldMat = m_enemy->GetWorldMatrix();
}


// �X�V����
void EnemyApproaching::Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos)
{
	//// elapsedTime�g��Ȃ����ǃG���[�o���Ȃ��ł˂��ĕ�
	//UNREFERENCED_PARAMETER(elapsedTime);
	
	// ���v�̎��Ԃ��v�Z����
	m_totalSeconds += elapsedTime;

	// �T�C���g�̌v�Z(�㉺�ړ�)
	m_position.y = Math::CalculatingSinWave(m_totalSeconds, m_amplitude, m_frequency);

	// �G�ɋ߂Â�����
	using namespace DirectX::SimpleMath;
	// �v���C���[�̍��W���擾
	Vector3 playerPos = m_enemy->GetPlayScene()->GetPlayer()->GetPosition();


	// �G���猩���v���C���[�̈ʒu���v�Z����
	m_angle = Math::CalculationAngle(m_position, playerPos);
	
	// ��]�s��̍쐬
	Matrix angleMat  = Matrix::CreateScale(Enemy::ENEMY_SCALE)
					*= Matrix::CreateRotationY(-m_angle);
	// �O���Ɉړ�
	m_position += Vector3::Transform(m_velocity, angleMat);


	// �̂̋��E���̈ʒu���X�V
	m_boundingSphereBody.Center = m_position;
	m_boundingSphereBody.Center.y = 0;

	// 2�b�o�߂őҋ@���[�V�����ɕύX
	if (m_totalSeconds >= 2.f)
	{
		m_enemy->ChangeState(m_enemy->GetEnemyIdling());
	}

	// ��]�p��ݒ肷��
	m_enemy->SetAngle(m_angle);

	// ���W��ݒ肷��
	m_enemy->SetPosition(m_position);
}


// �v���C���[�̑̂Ƃ̓����蔻����s��
void EnemyApproaching::CheckHitPlayerBody()
{
}


// ����X�V����
void EnemyApproaching::PostUpdate()
{
	// ���[���h�s���S�̂ɐݒ肷��
	m_enemy->SetWorldMatrix(m_worldMat);
	// �G�̈ʒu��0�ŌŒ肷��
	m_position.y = 0.f;
	m_enemy->SetPosition(m_position);
}


// �`�揈��
void EnemyApproaching::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
	)
{
	UNREFERENCED_PARAMETER(context, states, view, projection, m_model);



	// ���\�[�X�̎擾
	CommonResources* resources = CommonResources::GetInstance();
	// �f�o�b�O�����uDebugString�v�ŕ\������
	auto debugString = resources->GetDebugString();
}


void EnemyApproaching::Finalize()
{
}