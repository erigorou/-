/// ---------------------------
///
/// �S�̋��_�̑ҋ@���
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
#include "Game/Weapon/Cudgel/Header/Cudgel_Idling.h"
#include "Game/Weapon/Cudgel/Header/Cudgel.h"

// �R���X�g���N�^
Cudgel_Idling::Cudgel_Idling(Cudgel* cudgel)
	:
	m_cudgel(cudgel),
	m_position(0.0f, 0.0f, 0.0f),
	m_velocity(0.0f, 0.0f, 0.0f),
	m_angle(0.0f),
	m_worldMatrix(DirectX::SimpleMath::Matrix::Identity),
	m_model(nullptr)
{
}

// �f�X�g���N�^
Cudgel_Idling::~Cudgel_Idling()
{
}


// ����������
void Cudgel_Idling::Initialize()
{
	using namespace DirectX::SimpleMath;
	// ���f�����擾
	m_model = m_cudgel->GetModel();

	// ���[���h�s���������
	m_worldMatrix = Matrix::Identity;
}


// ���O����
void Cudgel_Idling::PreUpdate()
{
}

// �X�V����
void Cudgel_Idling::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;
	UNREFERENCED_PARAMETER(elapsedTime);

	// �G���擾
	auto enemy = m_cudgel->GetPlayScene()->GetEnemy();

	// �S�̍��W���擾
	m_position = enemy->GetPosition();
	// �S�̉�]���擾
	m_angle	= enemy->GetAngle();

	// ���[���h�s����v�Z����
	m_worldMatrix = Matrix::CreateScale(Cudgel::CUDGEL_SCALE)		// �傫���̐ݒ�@���@���Z�b�g
		*= Matrix::CreateTranslation(Vector3(14.0f, 2.0f, 0.0f))	// ���_�̈ʒu���炷�������炷
		*= Matrix::CreateRotationY(-m_angle)						// ���S�̂̉�]���s��
		*= Matrix::CreateTranslation(m_position);					// �S�̍��W�Ɉړ�����
}

// ���㏈��
void Cudgel_Idling::PostUpdate()
{
}

// �`�揈��
void Cudgel_Idling::Render(ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	CommonResources* resources = CommonResources::GetInstance();

	// ���f����`�悷��
	m_model->Draw(context, *states, m_worldMatrix, view, projection);

	auto debugString = resources->GetDebugString();
	debugString->AddString("Cudgel, %f : %f : %f", m_position.x, m_position.y, m_position.z);
}


// �I������
void Cudgel_Idling::Finalize()
{
}