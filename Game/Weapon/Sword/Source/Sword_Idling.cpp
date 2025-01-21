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
#include "Libraries/MyLib/Collision.h"

#include "Game/Player/Player.h"
#include "Game/Weapon/Sword/Header/Sword_Idling.h"

// -----------------------
// �R���X�g���N�^
// -----------------------
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


// -----------------------
// �f�X�g���N�^
// -----------------------
Sword_Idling::~Sword_Idling()
{
}


// -----------------------
// ����������
// -----------------------
void Sword_Idling::Initialize()
{
	using namespace DirectX::SimpleMath;
	// ���f�����擾
	m_model = m_sword->GetModel();
	// ���[���h�s���������
	m_worldMatrix = Matrix::Identity;
}


// -----------------------
// ���O����
// -----------------------
void Sword_Idling::PreUpdate()
{
	m_sword->SetAttackFlag(false);
}


// -----------------------
// �X�V����
// -----------------------
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
	
	m_worldMatrix
		// �X����
		*= SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(-90.0f))				// ��]
		*= SimpleMath::Matrix::CreateRotationX(XMConvertToRadians(90.0f))				// ��]
		// ��̈ʒu�Ɉړ�����
		*= SimpleMath::Matrix::CreateTranslation(Sword::SWORD_DIR_FOR_PLAYER)
		// �v���C���̈ړ�����t�^
		*= SimpleMath::Matrix::CreateRotationY(-m_angle)							// �v���C���̉�]��t�^
		*= SimpleMath::Matrix::CreateTranslation(m_position);						// �v���C���̈ʒu�ɐݒ肷��

	m_sword->SetCollisionPosition(m_worldMatrix);
}


// -----------------------
// ���㏈��
// -----------------------
void Sword_Idling::PostUpdate()
{
}


// -----------------------
// �I������
// -----------------------
void Sword_Idling::Finalize()
{
}


// -----------------------
// �����蔻�菈��
// -----------------------
void Sword_Idling::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}
