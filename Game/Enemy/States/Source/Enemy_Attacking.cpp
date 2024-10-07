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
	m_model = model;										// ���f���̎擾
}


// ���O�X�V����
void Enemy_Attacking::PreUpdate()
{
	// �o�ߎ��Ԃ�������
	m_totalSeconds = 0.0f;
	// ����̃X�e�[�g��ύX
	auto cudgel = m_enemy->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetAttacking());
}


// �X�V����
void Enemy_Attacking::Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos)
{
	using namespace DirectX::SimpleMath;
	m_totalSeconds += elapsedTime;

	if (m_totalSeconds >= 2.5f)
		m_enemy->ChangeState(m_enemy->GetEnemyIdling());	// �ҋ@��ԂɑJ��
}


// �v���C���[�̑̂Ƃ̓����蔻����s��
void Enemy_Attacking::CheckHitPlayerBody()
{

}


// ����X�V����
void Enemy_Attacking::PostUpdate()
{
	// ����̃X�e�[�g��ύX����
	auto cudgel = m_enemy->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());
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

#ifdef _DEBUG
	// ���\�[�X�̎擾
	CommonResources* resources = CommonResources::GetInstance();
	auto debugString = resources->GetDebugString();
	debugString->AddString("enemyAngle : %f", m_angle);
#endif // _DEBUG
}


void Enemy_Attacking::Finalize()
{
}