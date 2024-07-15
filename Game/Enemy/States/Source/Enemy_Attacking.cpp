#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Header/Cudgel.h"
#include "Game/Enemy/States/Header/Enemy_Attacking.h"


// �R���X�g���N�^
Enemy_Attacking::Enemy_Attacking(Enemy* enemy)
	:m_model()
	,m_angle(0.f)
	,m_enemy(enemy)
	,m_totalSeconds()
{
}


// �f�X�g���N�^
Enemy_Attacking::~Enemy_Attacking()
{
}


// ����������
void Enemy_Attacking::Initialize(DirectX::Model* model)
{
	// ���f���̎擾
	m_model = model;

	// �̂̓����蔻��̐���
	m_boundingSphereBody = DirectX::BoundingSphere();
	// �̂̓����蔻��̃T�C�Y����W��ݒ�
	m_boundingSphereBody.Radius = Enemy::ENEMY_SCALE * 12.f;
}


// ���O�X�V����
void Enemy_Attacking::PreUpdate()
{
	// �o�ߎ��Ԃ�������
	m_totalSeconds = 0.0f;
	// ����̃X�e�[�g��ύX
	auto cudgel = m_enemy->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());

}


// �X�V����
void Enemy_Attacking::Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos)
{
	using namespace DirectX::SimpleMath;

	// �o�ߎ��Ԃ̌v�Z
	m_totalSeconds += elapsedTime;


	if (m_totalSeconds >= 2.5f)
	{
		// 2.5�b�őҋ@��ԂɕύX
		m_enemy->ChangeState(m_enemy->GetEnemyIdling());
	}
}


// �v���C���[�̑̂Ƃ̓����蔻����s��
void Enemy_Attacking::CheckHitPlayerBody()
{

}


// ����X�V����
void Enemy_Attacking::PostUpdate()
{
	// �C���_������΂����ɋL��
}


// �`�揈��
void Enemy_Attacking::Render(
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

	// ���\�[�X�̎擾
	CommonResources* resources = CommonResources::GetInstance();

	// �f�o�b�O�����uDebugString�v�ŕ\������
	auto debugString = resources->GetDebugString();
	debugString->AddString("enemyAngle : %f", m_angle);
}


void Enemy_Attacking::Finalize()
{
}