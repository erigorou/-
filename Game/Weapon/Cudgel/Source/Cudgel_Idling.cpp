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
	using namespace DirectX;
	UNREFERENCED_PARAMETER(elapsedTime);

	// �v���C���[�̍��W���擾
	m_position = m_cudgel->GetPlayScene()->GetPlayer()->GetPosition();
	// �v���C���[�̉�]���擾
	m_angle = m_cudgel->GetPlayScene()->GetPlayer()->GetAngle();

	// ���[���h�s����X�V����
	m_worldMatrix = Matrix::CreateScale(Cudgel::CUDGEL_SCALE);							// �T�C�Y�̐ݒ�
	m_worldMatrix *= SimpleMath::Matrix::CreateTranslation(Vector3(4.0f, 0.5f, 0.0f))	// ���_�ŁA�����������炷
		*= SimpleMath::Matrix::CreateRotationY(-m_angle)								// ��]���s��
		*= SimpleMath::Matrix::CreateTranslation(m_position);							// �v���C���̈ʒu�ɐݒ肷��
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