/// ---------------------------
///
/// �S�̋��_�̍U�����
/// 
/// ---------------------------

#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/Collision.h"

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Weapon/Cudgel/Header/Cudgel_Attacking.h"
#include "Game/Weapon/Cudgel/Header/Cudgel.h"


// �Œ�l ==================================
const float Cudgel_Attacking::CHARGE_TIME = 1.0f;	// �U�肩��������
const float Cudgel_Attacking::WINDUP_TIME = 1.4f;	// �ҋ@
const float Cudgel_Attacking::ATTACK_TIME = 1.7f;	// �~�艺�낵

const Vector3 Cudgel_Attacking::ARM_LENGTH = Vector3(0.0f, 4.0f, 0.0f);
const Vector3 Cudgel_Attacking::ZERO_DIREC = Vector3(6.0f, 1.0f, 0.0f);


// �R���X�g���N�^
Cudgel_Attacking::Cudgel_Attacking(Cudgel* cudgel)
	:
	m_cudgel(cudgel)
	, m_position(0.0f, 0.0f, 0.0f)
	, m_velocity(0.0f, 0.0f, 0.0f)
	, m_angleRL(0.0f)
	, m_angleUD(0.0f)
	, m_totalSeconds(0.0f)
	, m_worldMatrix(DirectX::SimpleMath::Matrix::Identity)
	, m_model(nullptr)
{
}

// �f�X�g���N�^
Cudgel_Attacking::~Cudgel_Attacking()
{
}


// ����������
void Cudgel_Attacking::Initialize()
{
	using namespace DirectX::SimpleMath;
	// ���f�����擾
	m_model = m_cudgel->GetModel();

	// ���[���h�s���������
	m_worldMatrix = Matrix::Identity;

	// ���f���̑傫���ɍ��킹��OBB��ݒ肷��
	m_originalBox = Collision::Get_BoundingOrientedBox_FromMODEL(m_model);
}


// ���O����
void Cudgel_Attacking::PreUpdate()
{
	// �o�ߎ��Ԃ̏�����
	m_totalSeconds = 0.0f;
	m_angleUD = 0.0f;
}



// �X�V����
void Cudgel_Attacking::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;

	// ���v���Ԃ��v��
	m_totalSeconds += elapsedTime;

	auto enemy = m_cudgel->GetPlayScene()->GetEnemy();
	m_position = enemy->GetPosition();	// �G�̍��W���擾
	m_angleRL = enemy->GetAngle();		// �G�̉�]���擾


	// -----------------------------------------------------------------
	// �G�̍U���̗���
	// �U�肩�����i1�b�j�� �ҋ@�i0.4�b�j�� �~�艺�낷(0.3�b) �� �㌄
	// -----------------------------------------------------------------

	if (m_totalSeconds < CHARGE_TIME)
		m_angleUD = XMConvertToRadians(-20.0f * (m_totalSeconds / 0.5f)); // �グ��p�x��20�x�ɐݒ�
	
	else if (m_totalSeconds > WINDUP_TIME && m_totalSeconds < ATTACK_TIME)
		m_angleUD = XMConvertToRadians(-30.0f + 125.0f * ((m_totalSeconds - 1.4f) / 0.3f)); // 85�x�U�艺�낷

	else
	{
	}

	// ���[���h�s����X�V����
	m_worldMatrix = Matrix::CreateScale(Cudgel::CUDGEL_SCALE);		// �T�C�Y�̐ݒ� & ������
	m_worldMatrix
		*= Matrix::CreateTranslation(Cudgel_Attacking::ZERO_DIREC)	// ���_����ړ�����
		*= Matrix::CreateRotationX(-m_angleUD)						// �c��]���s��
		*= Matrix::CreateRotationY(-m_angleRL)						// ����]���s��
		*= Matrix::CreateTranslation(Cudgel_Attacking::ARM_LENGTH)	// �r��������̂Ƃ��Čv�Z���s��
		*= Matrix::CreateTranslation(m_position);					// �v���C���̈ʒu�ɐݒ肷��

	// �����蔻��𔽉f������
	m_originalBox.Transform(m_boundingBox, m_worldMatrix);
}

// ���㏈��
void Cudgel_Attacking::PostUpdate()
{
}


// �`�揈��
void Cudgel_Attacking::Render(ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	CommonResources* resources = CommonResources::GetInstance();

	// ���f����`�悷��
	m_model->Draw(context, *states, m_worldMatrix, view, projection);

#ifdef _DEBUG
	auto debugString = resources->GetDebugString();
	debugString->AddString("Cudgel, %f : %f : %f", m_position.x, m_position.y, m_position.z);
	debugString->AddString("angleUD, %f", m_angleUD);
#endif // _DEBUG
}


// �I������
void Cudgel_Attacking::Finalize()
{
}