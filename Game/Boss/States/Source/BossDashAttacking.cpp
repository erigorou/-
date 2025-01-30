#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries//MyLib/EasingFunctions.h"

#include "Game/Boss/Boss.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "../../States/Header/BossDashAttacking.h"


// --------------------
// �R���X�g���N�^
// --------------------
BossDashAttacking::BossDashAttacking(Boss* boss)
	:
	m_angle(0.0f),
	m_bodyTilt(0.0f),
	m_boss(boss),
	m_totalSeconds()
{
}


// --------------------
// �f�X�g���N�^
// --------------------
BossDashAttacking::~BossDashAttacking()
{
}


// --------------------
// ����������
// --------------------
void BossDashAttacking::Initialize()
{
}


// --------------------
// ���O�X�V����
// --------------------
void BossDashAttacking::PreUpdate()
{
	// �o�ߎ��Ԃ�������
	m_totalSeconds = 0.0f;

	// ��̃X�e�[�g��ύX
	m_boss->SetFace(m_boss->GetFaceAttacking());

	// �ŏ��͍U�����ł͂Ȃ�
	m_isAttacking = false;
}


// --------------------
// �X�V����
// --------------------
void BossDashAttacking::Update(const float& elapsedTime)
{
	// �o�ߎ��Ԃ��X�V
	m_elapsedTime = elapsedTime;
	m_totalSeconds += m_elapsedTime;

	// �G�̋������X�V����
	UpdateAction();

	// �ҋ@��ԂɑJ��
	if (m_totalSeconds >= TOTAL_TIME)
		m_boss->ChangeState(m_boss->GetBossIdling());
}


// --------------------
// �G�̋����X�V����
// --------------------
void BossDashAttacking::UpdateAction()
{
	// ���߃��[�V�����̎�
	if (m_totalSeconds		<= CHARGE_TIME)	ChargeAction();	// ����
	else if (m_totalSeconds <= DASH_TIME)	DashAction();	// �_�b�V��
	else if (m_totalSeconds <= WAIT_TIME)	WaitAction();	// �ҋ@
	else if (m_totalSeconds <= RETURN_TIME)	ReturnAction();	// ���ɖ߂�

	// �Փˉ\���ǂ���
	m_boss->GetPlayScene()->GetPlayer()->CanHit(m_isAttacking);
}


// --------------------
// ����
// --------------------
void BossDashAttacking::ChargeAction()
{
	// �v���C���[�̍��W���擾
	Vector3 playerPos = m_boss->GetPlayScene()->GetPlayer()->GetPosition();
	// �G�̍��W���擾
	Vector3 parentPos = m_boss->GetPosition();
	// �G���猩���v���C���[�̈ʒu��ݒ肷��
	m_angle = Math::CalculationAngle(parentPos, playerPos);
	m_rotMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(-m_angle);
	m_boss->SetAngle(m_angle);

	// �C�[�W���O�p�̕ϐ�
	float t = m_totalSeconds / CHARGE_TIME;
	// �̂̌X���̊p�x�ݒ�
	m_bodyTilt = DirectX::XMConvertToRadians(-20 * Easing::easeOutBack(t));
	m_boss->SetBodyTilt(m_bodyTilt);
}


// --------------------
// �_�b�V��
// --------------------
void BossDashAttacking::DashAction()
{
	// �A�^�b�N��
	m_isAttacking = true;

	// ���݂̎��ԂɊ�Â��ăT�C���g�ŉ����x���v�Z
	float t = (m_totalSeconds - CHARGE_TIME) / (DASH_TIME - CHARGE_TIME);
	
	// ���W�̍X�V *
	float accelerationFactor = sin(static_cast<float>(t * M_PI)); // �T�C���g�ő��x��ω�

	// �G�̌����Ɋ�Â��đO�������v�Z
	Vector3 position = m_boss->GetPosition();
	m_velocity = Vector3(0, 0, -MAX_SPEED * accelerationFactor);
	position += Vector3::Transform(m_velocity, m_rotMatrix) * m_elapsedTime;

	// �T�C���g�ŏ㉺�^��
	float y = fabsf(sin(t * 15.0f)) * accelerationFactor;
	position.y = y;

	// �v�Z�������W��{�̂ɐݒ肷��
	m_boss->SetPosition(position);

	// �X���̍X�V *
	// �v���C���[���X����
	m_bodyTilt = DirectX::XMConvertToRadians(-20 + 40 * Easing::easeOutBack(t));
	m_boss->SetBodyTilt(m_bodyTilt);
}


// --------------------
// �ҋ@
// --------------------
void BossDashAttacking::WaitAction()
{
	// �A�^�b�N�I���
	m_isAttacking = false;

	// �C�[�W���O�Ɏg�p����b�����v�Z�i�b����Normalize)
	float t = (m_totalSeconds - DASH_TIME) / (WAIT_TIME - DASH_TIME);

	// �v���C���[���X���� *
	m_bodyTilt = DirectX::XMConvertToRadians(20 - 20 * Easing::easeOutBounce(t));
	m_boss->SetBodyTilt(m_bodyTilt);
}


// --------------------
// ���ɖ߂�
// --------------------
void BossDashAttacking::ReturnAction()
{
	// ���Ԃ̐��K��
	float t = (m_totalSeconds - WAIT_TIME) / (RETURN_TIME - WAIT_TIME);

	// �v���C���[�̍��W���擾
	Vector3 playerPos = m_boss->GetPlayScene()->GetPlayer()->GetPosition();
	// �G�̍��W���擾
	Vector3 parentPos = m_boss->GetPosition();
	// �G���猩���v���C���[�̈ʒu��ݒ肷��
	float angle = Math::CalculationAngle(parentPos, playerPos);

	m_angle = Math::LerpFloat(m_angle, angle, t);

	m_rotMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(-m_angle);
	m_boss->SetAngle(m_angle);
}



// --------------------
// ����X�V����
// --------------------
void BossDashAttacking::PostUpdate()
{
	// ����̃X�e�[�g��ύX����
	auto cudgel = m_boss->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());

	// ��̃X�e�[�g��ύX
	m_boss->SetFace(m_boss->GetFaceIdling());
}


// --------------------
// �I������
// --------------------
void BossDashAttacking::Finalize()
{
}