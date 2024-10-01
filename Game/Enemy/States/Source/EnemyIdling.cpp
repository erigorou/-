#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

// �֐����g�p����̂ɕK�v�Ȉ���
#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Header/Cudgel.h"

// �w�b�_�[�t�@�C��
#include "Game/Enemy/States/Header/EnemyIdling.h"


// �R���X�g���N�^
EnemyIdling::EnemyIdling(Enemy* enemy)
	:
	m_angle(0.f),
	m_enemy(enemy),
	m_totalSeconds()
{
}


// �f�X�g���N�^
EnemyIdling::~EnemyIdling()
{
}


// ����������
void EnemyIdling::Initialize(DirectX::Model* model)
{
	m_model = model;
	m_boundingSphereBody = DirectX::BoundingSphere();			// ���f�����擾���A
	m_boundingSphereBody.Radius = Enemy::ENEMY_SCALE * 12.f;	// 
}


// ���O�X�V����
void EnemyIdling::PreUpdate()
{
	// �o�ߎ��Ԃ�������
	m_totalSeconds = 0;
	// ����̃X�e�[�g��ύX
	auto cudgel = m_enemy->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());
}


// �X�V����
void EnemyIdling::Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos)
{
	using namespace DirectX::SimpleMath;
	m_totalSeconds += elapsedTime;

	// �v���C���[�̍��W���擾
	Vector3 playerPos = m_enemy->GetPlayScene()->GetPlayer()->GetPosition();
	// �G���猩���v���C���[�̈ʒu���v�Z����
	m_angle = Math::CalculationAngle(parentPos, playerPos);

	// �̂̋��E���̈ʒu���X�V
	m_boundingSphereBody.Center = parentPos;

	// ��]�p��ݒ肷��
	m_enemy->SetAngle(m_angle);

	if (m_totalSeconds >= 2.f)
	{
		m_enemy->ChangeState(m_enemy->GetEnemySweeping());	// �ガ����

		// �߂��������ƒǏ]���s��Ȃ�
		// �v���C���[�Ƃ̋������v�Z
		float distance = Vector3::Distance(parentPos, playerPos);

		//if (distance > 25.0f)
		//	m_enemy->ChangeState(m_enemy->GetEnemyApproaching());	// �Ǐ]
		//else
		//	m_enemy->ChangeState(m_enemy->GetEnemyAttacking());		// �ҋ@
	}
}


// �v���C���[�̑̂Ƃ̓����蔻����s��
void EnemyIdling::CheckHitPlayerBody()
{

}


// ����X�V����
void EnemyIdling::PostUpdate()
{
	// �C���_������΂����ɋL��
}


// �`�揈��
void EnemyIdling::Render(
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


#ifdef _DEBUG
	// �f�o�b�O�����uDebugString�v�ŕ\������
	auto debugString = resources->GetDebugString();
	debugString->AddString("enemyAngle : %f", m_angle);
#endif // _DEBUG
}


void EnemyIdling::Finalize()
{
}