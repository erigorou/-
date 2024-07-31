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

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Weapon/Cudgel/Header/Cudgel_Attacking.h"
#include "Game/Weapon/Cudgel/Header/Cudgel.h"

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

	// �G���擾
	auto enemy = m_cudgel->GetPlayScene()->GetEnemy();
	// �G�̍��W���擾
	m_position = enemy->GetPosition();
	// �G�̉�]�p���擾
	m_angleRL = enemy->GetAngle();


	// 0.5�b��ɂ����āA
	if (m_totalSeconds < 1.0f)
	{
		m_angleUD = XMConvertToRadians(-45.0f * (m_totalSeconds / 0.5f)); // �グ��p�x��45�x�ɐݒ�
	}

	// 0.,2�b�Ԃ̒x��
	else if (m_totalSeconds < 1.4f){}

	// 0.2�b�U�艺�낷�B
	else if (m_totalSeconds < 1.7f)
	{
		// ����0.2�b�ԂŐU�艺�낷
		m_angleUD = XMConvertToRadians(-30.0f + 90.0f * ((m_totalSeconds - 1.4f) / 0.2f)); // 90�x�U�艺�낷
	}


	// ���[���h�s����X�V����
	m_worldMatrix = Matrix::CreateScale(Cudgel::CUDGEL_SCALE);	// �T�C�Y�̐ݒ� & ������
	m_worldMatrix
		*= Matrix::CreateTranslation(Vector3(0.0f, 1.0f, 0.0f))			// ���_���炷�������炷
		*= Matrix::CreateRotationX(-m_angleUD)							// �c��]���s��
		*= Matrix::CreateTranslation(Cudgel::DIRECTION_ENEMY)			// �v���C���[�̈ʒu�ɂ��炷
		*= Matrix::CreateRotationY(-m_angleRL)							// ����]���s��
		*= Matrix::CreateTranslation(m_position);						// �v���C���̈ʒu�ɐݒ肷��
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