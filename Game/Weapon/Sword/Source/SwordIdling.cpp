/// ---------------------------
///
/// �v���C���[�̌��̑ҋ@���
/// 
/// ---------------------------

#include "pch.h"
#include <cassert>
#include "Game/Weapon/Sword/Sword.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/Collision.h"
#include "Game/Messenger/EventMessenger.h"

#include "Game/Player/Player.h"
#include "Game/Weapon/Sword/Header/SwordIdling.h"

// -----------------------
// �R���X�g���N�^
// -----------------------
SwordIdling::SwordIdling(Sword* sword)
	:
	m_sword(sword),
	m_position{},
	m_velocity{},
	m_angle{},
	m_worldMatrix(DirectX::SimpleMath::Matrix::Identity)
{
}

// -----------------------
// �f�X�g���N�^
// -----------------------
SwordIdling::~SwordIdling()
{
}

// -----------------------
// ����������
// -----------------------
void SwordIdling::Initialize()
{
	// ���[���h�s���������
	m_worldMatrix = DirectX::SimpleMath::Matrix::Identity;
}

// -----------------------
// ���O����
// -----------------------
void SwordIdling::PreUpdate()
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
void SwordIdling::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	// �v���C���[���擾
	auto player = m_sword->GetPlayer();
	// �v���C���[�̍��W���擾
	m_position = player->GetPosition();
	// �v���C���[�̉�]���擾
	m_angle = player->GetAngle();

	// �N�I�[�^�j�I���ɂ���]�v�Z
	DirectX::SimpleMath::Quaternion baseRotation =
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitY, DirectX::XMConvertToRadians(-90.0f)) *
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitX, DirectX::XMConvertToRadians(90.0f));

	DirectX::SimpleMath::Quaternion playerRotation =
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitY, DirectX::XMConvertToRadians(m_angle));

	// ��]�s��ɕϊ�
	DirectX::SimpleMath::Matrix baseRotationMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(baseRotation);

	DirectX::SimpleMath::Matrix playerRotationMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(playerRotation);

	// ���[���h�s����X�V����
	m_worldMatrix = DirectX::SimpleMath::Matrix::CreateScale(Sword::SWORD_SCALE)
		* baseRotationMatrix // �܂�XY���̉�]��K�p
		* DirectX::SimpleMath::Matrix::CreateTranslation(Sword::SWORD_DIR_FOR_PLAYER) // ������ʒu��K�p
		* playerRotationMatrix // �Ō��m_angle��K�p
		* DirectX::SimpleMath::Matrix::CreateTranslation(m_position); // �Ō�Ƀv���C���[�̈ʒu��K�p

	m_sword->SetWorldMatrix(m_worldMatrix); // ���[���h�s���ݒ�
	m_sword->SetCollisionPosition(m_worldMatrix); // �����蔻��̈ʒu��ݒ�
}

// -----------------------
// ���㏈��
// -----------------------
void SwordIdling::PostUpdate()
{
}

// -----------------------
// �I������
// -----------------------
void SwordIdling::Finalize()
{
}

// -----------------------
// �����蔻�菈��
// -----------------------
void SwordIdling::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}
