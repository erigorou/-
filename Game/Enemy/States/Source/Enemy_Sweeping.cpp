#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Header/Cudgel.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Enemy/States/Header/Enemy_Sweeping.h"



// �R���X�g���N�^
Enemy_Sweeping::Enemy_Sweeping(Enemy* enemy)
	:m_model()
	, m_angle(0.f)
	, m_enemy(enemy)
	, m_totalSeconds()
{
	m_easying = std::make_unique<EasyingFunctions>();
}


// �f�X�g���N�^
Enemy_Sweeping::~Enemy_Sweeping()
{
}


// ����������
void Enemy_Sweeping::Initialize(DirectX::Model* model)
{
	// ���f���̎擾
	m_model = model;

	m_boundingSphereBody = DirectX::BoundingSphere();		// ���E���̐����Ɛݒ�
	m_boundingSphereBody.Radius = Enemy::ENEMY_SCALE * 12.f;
}



// ���O�X�V����
void Enemy_Sweeping::PreUpdate()
{
	// �o�ߎ��Ԃ̏�����
	m_totalSeconds = 0.0f;
	// �����蔻����v���C���[�̈ʒu�ɏC��
	m_boundingSphereBody.Center = m_enemy->GetPosition();
	m_angle = DirectX::XMConvertToDegrees(m_enemy->GetAngle());

	auto cudgel = m_enemy->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetSweeping());			// ���_�̃X�e�[�g��ύX
}


// �X�V����
void Enemy_Sweeping::Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos)
{
	using namespace DirectX::SimpleMath;

	// �o�ߎ��Ԃ̌v�Z
	m_totalSeconds += elapsedTime;

	float t = 0.0f;  // ���K�����ꂽ�o�ߎ���
	static float targetAngle = 0.0f;

	float fastTime = 1.0f;
	float secondTime = 1.3f;
	float thirdTime = 1.8f;

	// �ŏ���0.2�b�� -20�x���� 0�x�ɉ�]
	if (m_totalSeconds <= fastTime) {
		t = m_totalSeconds / fastTime;  // 0 ~ 1 �ɐ��K��
		targetAngle = m_angle - 20.f * m_easying->easeOutCirc(t);  // -20�x���� 0�x�ɉ�]

		targetAngle -= 180;
	}
	else if (m_totalSeconds >= secondTime && m_totalSeconds <= thirdTime) {
		t = (m_totalSeconds - secondTime) / (thirdTime - secondTime);  // 0 ~ 1 �ɐ��K��
		targetAngle = m_angle - 20.f + 20.f * m_easying->easeOutBack(t);  // -20�x���� 0�x�ɉ�]

		targetAngle -= 180;
	}

	if(m_totalSeconds > 2.5f)
	{
		// ��]�����������玟�̃X�e�[�g�Ɉڍs
		m_enemy->ChangeState(m_enemy->GetEnemyIdling());
	}

	m_enemy->SetAngle(DirectX::XMConvertToRadians(targetAngle + 180));

	// �����蔻��̈ʒu�𒲐�����
	m_boundingSphereBody.Center = parentPos;
}


// ����X�V����
void Enemy_Sweeping::PostUpdate()
{
}

// �v���C���[�̑̂Ƃ̓����蔻����s��
void Enemy_Sweeping::CheckHitPlayerBody()
{

}


// �`�揈��
void Enemy_Sweeping::Render(
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


void Enemy_Sweeping::Finalize()
{
}
