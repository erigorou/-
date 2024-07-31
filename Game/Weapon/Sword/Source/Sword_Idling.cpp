/// ---------------------------
///
/// �v���C���[�̌��̑ҋ@���
/// 
/// ---------------------------

#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Player/Player.h"
#include "Game/Weapon/Sword/Header/Sword_Idling.h"

// �R���X�g���N�^
Sword_Idling::Sword_Idling(Sword* sword)
	:
	m_sword(sword),
	m_position(0.0f, 0.0f, 0.0f),
	m_velocity(0.0f, 0.0f, 0.0f),
	m_angle(0.0f),
	m_worldMatrix(DirectX::SimpleMath::Matrix::Identity),
	m_model(nullptr)
{
}

// �f�X�g���N�^
Sword_Idling::~Sword_Idling()
{
}


// ����������
void Sword_Idling::Initialize()
{
	using namespace DirectX::SimpleMath;
	// ���f�����擾
	m_model = m_sword->GetModel();
	// ���[���h�s���������
	m_worldMatrix = Matrix::Identity;
	// �����蔻��
	m_boundingBox = DirectX::BoundingOrientedBox();
}


// ���O����
void Sword_Idling::PreUpdate()
{
}

// �X�V����
void Sword_Idling::Update(float elapsedTime)
{
	using namespace DirectX;
	UNREFERENCED_PARAMETER(elapsedTime);

	// �v���C���[�̍��W���擾
	m_position = m_sword->GetPlayScene()->GetPlayer()->GetPosition();
	// �v���C���[�̉�]���擾
	m_angle = m_sword->GetPlayScene()->GetPlayer()->GetAngle();

	// ���[���h�s����X�V����
	m_worldMatrix = Matrix::CreateScale(Sword::SWORD_SCALE);								// �T�C�Y�̐ݒ�
	m_worldMatrix *= SimpleMath::Matrix::CreateTranslation(Sword::SWORD_DIR_FOR_PLAYER)		// ���_�ŁA�����������炷
		*= SimpleMath::Matrix::CreateRotationY(-m_angle)									// ��]���s��
		*= SimpleMath::Matrix::CreateTranslation(m_position);								// �v���C���̈ʒu�ɐݒ肷��
}

// ���㏈��
void Sword_Idling::PostUpdate()
{
}

// �`�揈��
void Sword_Idling::Render(ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	CommonResources* resources = CommonResources::GetInstance();

	// ���f����`�悷��
	m_model->Draw(context, *states, m_worldMatrix, view, projection);

	auto debugString = resources->GetDebugString();
	debugString->AddString("sword, %f : %f : %f", m_position.x, m_position.y, m_position.z);
}


// �I������
void Sword_Idling::Finalize()
{
}