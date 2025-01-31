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
#include "Game/Messenger/EventMessenger.h"

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
	m_worldMatrix(DirectX::SimpleMath::Matrix::Identity)
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
	// ���[���h�s���������
	m_worldMatrix = DirectX::SimpleMath::Matrix::Identity;
}


// -----------------------
// ���O����
// -----------------------
void Sword_Idling::PreUpdate()
{
	m_sword->SetAttackFlag(false);

	// �G�S�̂��Փ˕s�ɂ���
	bool canHit = false;

	// �Փˉ\�t���O��G�S�̂ɕt�^
	EventMessenger::Execute("EnemyCanHit", &canHit);
}


// -----------------------
// �X�V����
// -----------------------
void Sword_Idling::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	// �v���C���[���擾
	auto player = m_sword->GetPlayer();
	// �v���C���[�̍��W���擾
	m_position = player->GetPosition();
	// �v���C���[�̉�]���擾
	m_angle = player->GetAngle();

	// ���[���h�s����X�V����
	m_worldMatrix = Matrix::CreateScale(Sword::SWORD_SCALE);								// �T�C�Y�̐ݒ�
	
	m_worldMatrix
		// �X����
		*= DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(-90.0f))// ��]
		*= DirectX::SimpleMath::Matrix::CreateRotationX(DirectX::XMConvertToRadians(90.0f))	// ��]
		*= DirectX::SimpleMath::Matrix::CreateTranslation(Sword::SWORD_DIR_FOR_PLAYER)	// �v���C���̈ʒu�ɐݒ肷��
		*= DirectX::SimpleMath::Matrix::CreateRotationY(-m_angle)						// �v���C���̉�]��t�^
		*= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);					// �v���C���̈ʒu�ɐݒ肷��

	m_sword->SetWorldMatrix(m_worldMatrix);			// ���[���h�s���ݒ�
	m_sword->SetCollisionPosition(m_worldMatrix);	// �����蔻��̈ʒu��ݒ�
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
