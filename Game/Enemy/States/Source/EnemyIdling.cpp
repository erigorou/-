#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

// �֐����g�p����̂ɕK�v�Ȉ���
#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"

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
}


// ���O�X�V����
void EnemyIdling::PreUpdate()
{
	// �o�ߎ��Ԃ�������
	m_totalSeconds = 0;
	// ����̃X�e�[�g��ύX
	auto cudgel = m_enemy->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());

	// ��̃X�e�[�g��ύX
	m_enemy->SetFace(m_enemy->GetFaceIdling());
}


// �X�V����
void EnemyIdling::Update(const float& elapsedTime)
{
	using namespace DirectX::SimpleMath;
	m_totalSeconds += elapsedTime;

	// �v���C���[�̍��W���擾
	Vector3 playerPos = m_enemy->GetPlayScene()->GetPlayer()->GetPosition();
	// �G�̍��W���擾
	Vector3 parentPos = m_enemy->GetPosition();

	// �G���猩���v���C���[�̈ʒu���v�Z����
	m_angle = Math::CalculationAngle(parentPos, playerPos);

	// ��]�p��ݒ肷��
	m_enemy->SetAngle(m_angle);

	if (m_totalSeconds >= 2.f)
	{
		// �߂��������ƒǏ]���s��Ȃ�
		float distance = Vector3::Distance(parentPos, playerPos);

		if (distance > 25.0f)
			m_enemy->ChangeState(m_enemy->GetEnemyApproaching());	// �Ǐ]
		else
		{
			int random = Math::RandomInt(0, 10);

			if (random <= 5)
				m_enemy->ChangeState(m_enemy->GetEnemySweeping());		// �|��
			else if(random == 8)
				m_enemy->ChangeState(m_enemy->GetEnemyAttacking());		// �U��
			else if(random == 10)
				m_enemy->ChangeState(m_enemy->GetEnemyIdling());		// �������Ȃ�
		}
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



#ifdef _DEBUG
	// ���\�[�X�̎擾
	CommonResources* resources = CommonResources::GetInstance();
	// �f�o�b�O�����uDebugString�v�ŕ\������
	auto debugString = resources->GetDebugString();
	debugString->AddString("enemyAngle : %f", m_angle);
#endif // _DEBUG
}


void EnemyIdling::Finalize()
{
}