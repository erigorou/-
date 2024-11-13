#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
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

	// ��̃X�e�[�g��ύX
	m_enemy->SetFace(m_enemy->GetFaceAttacking());
}


// �X�V����
void Enemy_Attacking::Update(const float& elapsedTime)
{
	using namespace DirectX::SimpleMath;
	m_totalSeconds += elapsedTime;

	// �v���C���[�̍��W���擾
	Vector3 playerPos = m_enemy->GetPlayScene()->GetPlayer()->GetPosition();
	// �G�̍��W���擾
	Vector3 parentPos = m_enemy->GetPosition();
	// �G���猩���v���C���[�̈ʒu���v�Z����
	m_angle = Math::CalculationAngle(parentPos, playerPos);

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

	// ��̃X�e�[�g��ύX
	m_enemy->SetFace(m_enemy->GetFaceIdling());
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
#endif // _DEBUG
}


void Enemy_Attacking::Finalize()
{
}