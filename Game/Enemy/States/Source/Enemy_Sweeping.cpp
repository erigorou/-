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


const float Enemy_Sweeping::CHARGE_TIME = 1.0f;	// �U�肩��������
const float Enemy_Sweeping::WINDUP_TIME = 1.3f;	// �ҋ@
const float Enemy_Sweeping::ATTACK_TIME = 1.8f;	// �ガ����
const float Enemy_Sweeping::END_TIME	= 2.5f;	// �I��
const float Enemy_Sweeping::ROTATE_ANGLE = 20.0f;	// �U�肩�����p�x


// ----------------------------------
// �R���X�g���N�^
// ----------------------------------
Enemy_Sweeping::Enemy_Sweeping(Enemy* enemy)
	:m_model()
	, m_angle(0.f)
	, m_enemy(enemy)
	, m_totalSeconds()
{
	m_easying = std::make_unique<EasyingFunctions>();
}


// ----------------------------------
// �f�X�g���N�^
// ----------------------------------
Enemy_Sweeping::~Enemy_Sweeping()
{
}


// ----------------------------------
// ����������
// ----------------------------------
void Enemy_Sweeping::Initialize(DirectX::Model* model)
{
	// ���f���̎擾
	m_model = model;

	m_boundingSphereBody = DirectX::BoundingSphere();		// ���E���̐����Ɛݒ�
	m_boundingSphereBody.Radius = Enemy::ENEMY_SCALE * 12.f;
}



// ----------------------------------
// ���O�X�V����
// ----------------------------------
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


// ----------------------------------
// �X�V����
// ----------------------------------
void Enemy_Sweeping::Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos)
{
	using namespace DirectX::SimpleMath;

	// �o�ߎ��Ԃ̌v�Z
	m_totalSeconds += elapsedTime;

	float t = 0.0f;						// �C�[�W���O�Ɏg�p����ϐ�
	static float targetAngle = 0.0f;	// �ڕW�̊p�x

	if (m_totalSeconds <= CHARGE_TIME)	// �J�n���炽�߃��[�V�������Ȃ�
	{
		t = m_totalSeconds / CHARGE_TIME;
		targetAngle = m_angle - ROTATE_ANGLE * m_easying->easeOutCirc(t);					// ���߃��[�V�������s��
	}

	else if (
		m_totalSeconds >= WINDUP_TIME &&	// �ҋ@���Ԃ��߂����@����
		m_totalSeconds <= ATTACK_TIME)		// �U�����Ԓ��Ȃ�
	{
		t = (m_totalSeconds - WINDUP_TIME) / (ATTACK_TIME - WINDUP_TIME);
		targetAngle = m_angle - ROTATE_ANGLE + ROTATE_ANGLE * m_easying->easeOutBack(t);	// �ガ�������[�V�������s��
	}


	if(m_totalSeconds > END_TIME)	m_enemy->ChangeState(m_enemy->GetEnemyIdling());		// �ҋ@��ԂɑJ��


	m_enemy->SetAngle(DirectX::XMConvertToRadians(targetAngle));							// �p�x��ݒ�

	// �����蔻��̈ʒu�𒲐�����
	m_boundingSphereBody.Center = parentPos;
}


// ----------------------------------
// ����X�V����
// ----------------------------------
void Enemy_Sweeping::PostUpdate()
{
}


// ----------------------------------
// �`�揈��
// ----------------------------------
void Enemy_Sweeping::CheckHitPlayerBody()
{

}


// ----------------------------------
// �`�揈��
// ----------------------------------
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


// ----------------------------------
// �I������
// ----------------------------------
void Enemy_Sweeping::Finalize()
{
}
