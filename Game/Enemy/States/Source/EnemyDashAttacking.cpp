#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "../../States/Header/EnemyDashAttacking.h"


// �R���X�g���N�^
EnemyDashAttacking::EnemyDashAttacking(Enemy* enemy)
	:m_model()
	, m_angle(0.f)
	, m_enemy(enemy)
	, m_totalSeconds()
{
}


// �f�X�g���N�^
EnemyDashAttacking::~EnemyDashAttacking()
{
}


// ����������
void EnemyDashAttacking::Initialize(DirectX::Model* model)
{
	m_model = model;										// ���f���̎擾
}


// ���O�X�V����
void EnemyDashAttacking::PreUpdate()
{
	// �o�ߎ��Ԃ�������
	m_totalSeconds = 0.0f;

	//// ����̃X�e�[�g��ύX
	//auto cudgel = m_enemy->GetPlayScene()->GetCudgel();
	//cudgel->ChangeState(cudgel->GetAttacking());

	// ��̃X�e�[�g��ύX
	m_enemy->SetFace(m_enemy->GetFaceAttacking());
}


// �X�V����
void EnemyDashAttacking::Update(const float& elapsedTime)
{
	m_totalSeconds += elapsedTime;

	// �G�̋������X�V����
	UpdateAction(elapsedTime);


	if (m_totalSeconds >= 4.3f)
		m_enemy->ChangeState(m_enemy->GetEnemyIdling());	// �ҋ@��ԂɑJ��
}

// --------------------
// �G�̋����X�V����
// --------------------
void EnemyDashAttacking::UpdateAction(float elapsedTime)
{

	// ���߃��[�V�����̎�
	if (m_totalSeconds <= CHARGE_TIME)		ChargeAction();	// ����
	else if (m_totalSeconds <= DASH_TIME)	DashAction(elapsedTime);	// �_�b�V��
	else if (m_totalSeconds <= WAIT_TIME)	WaitAction();	// �ҋ@
	else if (m_totalSeconds <= RETURN_TIME)	ReturnAction();	// ���ɖ߂�
}


// --------------------
// ����
// --------------------
void EnemyDashAttacking::ChargeAction()
{
	// �v���C���[�̍��W���擾
	Vector3 playerPos = m_enemy->GetPlayScene()->GetPlayer()->GetPosition();
	// �G�̍��W���擾
	Vector3 parentPos = m_enemy->GetPosition();
	// �G���猩���v���C���[�̈ʒu���v�Z����
	m_angle = Math::CalculationAngle(parentPos, playerPos);
	// �v���C���[�̕�����T������
	m_enemy->SetAngle(m_angle);
}


// --------------------
// �_�b�V��
// --------------------
void EnemyDashAttacking::DashAction(float elapsedTime)
{
	// ���݂̎��ԂɊ�Â��ăT�C���g�ŉ����x���v�Z
	float t = (m_totalSeconds - CHARGE_TIME) / (DASH_TIME - CHARGE_TIME);
	float accelerationFactor = sin(t * 2.0f); // �T�C���g�ő��x��ω�
	
	// �G�̍��W���擾
	Vector3 position = m_enemy->GetPosition();

	// �G�̌����Ɋ�Â��đO�������v�Z
	DirectX::SimpleMath::Matrix rotationMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(-m_angle);

	// ���x���v�Z
	Vector3 velocity = Vector3(0, 0, -MAX_SPEED * accelerationFactor);

	position += Vector3::Transform(velocity, rotationMatrix) * elapsedTime;



	// �G�̍��W��ݒ�
	m_enemy->SetPosition(position);
}


// --------------------
// �ҋ@
// --------------------
void EnemyDashAttacking::WaitAction()
{
}


// --------------------
// ���ɖ߂�
// --------------------
void EnemyDashAttacking::ReturnAction()
{
}





// ����X�V����
void EnemyDashAttacking::PostUpdate()
{
	// ����̃X�e�[�g��ύX����
	auto cudgel = m_enemy->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());

	// ��̃X�e�[�g��ύX
	m_enemy->SetFace(m_enemy->GetFaceIdling());
}


// �`�揈��
void EnemyDashAttacking::Render(
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


void EnemyDashAttacking::Finalize()
{
}