/// ---------------------------
///
/// �S�̋��_�̍U�����(�ガ����)
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
#include "Game/Sound/Sound.h"
#include "Game/Weapon/WeaponState.h"

#include "Game/Boss/Boss.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Weapon/Cudgel/Header/Cudgel_Sweeping.h"
#include "Game/Weapon/Cudgel/Cudgel.h"


// -----------------------------
// �R���X�g���N�^
// -----------------------------
Cudgel_Sweeping::Cudgel_Sweeping(Cudgel* cudgel)
	:
	m_cudgel(cudgel),
	m_position{},
	m_velocity{},
	m_angleRL{},
	m_angleUD{},
	m_parentAngleRL{},
	m_totalSeconds{},
	m_worldMatrix(DirectX::SimpleMath::Matrix::Identity),
	m_playSound(false),
	m_playerCanHit(false)
{
}


// ----------------------------
// �f�X�g���N�^
// ----------------------------
Cudgel_Sweeping::~Cudgel_Sweeping()
{
}


// ----------------------------
// ����������
// ----------------------------
void Cudgel_Sweeping::Initialize()
{
	// �{�X�̐ݒ�
	m_boss = m_cudgel->GetBoss();
}


// -----------------------------
// ���O����
// -----------------------------
void Cudgel_Sweeping::PreUpdate()
{
	// �o�ߎ��Ԃ̏�����
	m_totalSeconds = 0.0f;
	// ���_�����ɂ���
	m_angleUD = DirectX::XMConvertToRadians(INITIAL_ROTATE_UD);
	// ����]�̏�����
	m_angleRL = 0.0f;
	// �e�̊p�x���擾
	m_parentAngleRL = m_boss->GetAngle();
	// �G�t�F�N�g�p�̒��_����������
	m_rootPos.clear();
	// �G�t�F�N�g�p�̒��_����������
	m_tipPos.clear();
	// �T�E���h�̍Đ��t���O��false�ɂ���
	m_playSound = false;
}


// --------------------------------
// �X�V����
// --------------------------------
void Cudgel_Sweeping::Update(float elapsedTime)
{
	// ���v���Ԃ��v��
	m_totalSeconds += elapsedTime;

	// �A�j���[�V�������X�V����
	UpdateCudgelRotation();

	// ���[���h�s����v�Z
	CalculateModelMatrix();
	// ���[���擾����
	GetCudgelBothEnds();


	// ���[���h�s���ݒ肷��
	m_cudgel->SetWorldMatrix(m_worldMatrix);
	// �����蔻��p�̍s����擾
	m_collMatrix = m_worldMatrix;
	// �����蔻��̈ʒu�𒲐�����
	m_collMatrix._42 = 0.0f;
	m_cudgel->SetCollisionPosition(m_collMatrix);
}


// ------------------------------
// Cudgel�̏c���̉�]�̍X�V�֐�
// ------------------------------
void Cudgel_Sweeping::UpdateCudgelRotation()
{
	// �G�̍��W���擾
	m_position = m_boss->GetPosition();
	// ���x���Ńt���O��false�ɂ���
	m_playerCanHit = false;

	// ���߃��[�V����
	ChargeAnimation();
	// �U�����[�V����
	AttackAnimation();
	// �I�����[�V����
	EndAnimation();
	// �v���C���[�ɍU���\����ʒm
	EventMessenger::Execute(EventList::PlayerCanHit, &m_playerCanHit);
}



// --------------------------------
// ���߃��[�V����
// --------------------------------
void Cudgel_Sweeping::ChargeAnimation()
{
	// �U���O�̒��߂�A�j���[�V�������s��
	if (m_totalSeconds <= CHARGE_TIME)
	{
		// �b���𐳋K��
		float easing = m_totalSeconds / CHARGE_TIME;
		// �C�[�W���O���|������]�p���v�Z
		m_angleRL = -CHARGE_ROTATE_ANGLE * Easing::easeOutCirc(easing);
	}
}


// --------------------------------
// �U�����[�V����
// --------------------------------
void Cudgel_Sweeping::AttackAnimation()
{
	// �U�����[�V����
	if (Math::InTime(WINDUP_TIME, m_totalSeconds, ATTACK_TIME))
	{
		// �b���̐��K��
		float easing = (m_totalSeconds - WINDUP_TIME) / (ATTACK_TIME - WINDUP_TIME);
		// �C�[�W���O���|������]���v�Z
		m_angleRL = CHARGE_ROTATE_ANGLE + WINDUP_ROTATE_ANGLE * Easing::easeOutBack(easing);
		// �v���C���[�ɍU���\����ʒm
		m_playerCanHit = true;
		// SE�̍Đ�
		if (m_playSound == false)
		{
			// 1�x�����Đ�����
			Sound::PlaySE(Sound::SE_TYPE::BOSS_SWEEP);
			m_playSound = true;
		}
	}
}

// ---------------------------------
// �A�j���[�V�����I��
// ---------------------------------
void Cudgel_Sweeping::EndAnimation()
{
	if (m_totalSeconds > END_TIME)
	{
		// �X�e�[�g��ύX
		CudgelState state = CudgelState::Idle;
		EventMessenger::Execute(EventList::ChangeCudgelState, &state);
	}
}


// ----------------------------------
// �`��p���[���h�s����v�Z����֐�
// ----------------------------------
void Cudgel_Sweeping::CalculateModelMatrix()
{
	m_worldMatrix = Matrix::CreateScale(Cudgel::CUDGEL_SCALE);		// �T�C�Y�̐ݒ� & ������
	m_worldMatrix
		*= Matrix::CreateTranslation(Cudgel_Sweeping::ZERO_DIREC)	// �����ʒu�ւ̈ړ�
		*= CalculateAttackMatrix();									// �U�����[�V�������̌v�Z
}


// ----------------------------------------
// Cudgel�̃��[���h�s����v�Z����֐�
// ----------------------------------------
DirectX::SimpleMath::Matrix Cudgel_Sweeping::CalculateAttackMatrix()
{
	return 
			Matrix::CreateRotationX(-m_angleUD)																						// �c��]���s��
		*=  Matrix::CreateRotationY(DirectX::XMConvertToRadians(-m_angleRL) - m_parentAngleRL - DirectX::XMConvertToRadians(180))	// ����]���s��
		*=  Matrix::CreateTranslation(Cudgel_Sweeping::ARM_LENGTH)																	// �r�̒������ړ�
		*=  Matrix::CreateTranslation(m_position);																					// �Ō�ɓG�̈ʒu�ɐݒ�
}


// ----------------------------------------------------
// ���_�̍��{�ƒ��_�̍��W���擾����֐�
// ----------------------------------------------------
void Cudgel_Sweeping::GetCudgelBothEnds()
{
	DirectX::SimpleMath::Vector3 root;
	DirectX::SimpleMath::Vector3 tip;

	Matrix rootMat = Matrix::CreateTranslation(Cudgel_Sweeping::ZERO_DIREC);
	rootMat
		*= Matrix::CreateTranslation(Cudgel::CUDGEL_HADLE_POS)
		*= CalculateAttackMatrix();
	root = Vector3::Transform(Vector3::Zero, rootMat);		// ���f���̐�[�̈ʒu���擾

	
	Matrix tipMat = Matrix::CreateTranslation(Cudgel_Sweeping::ZERO_DIREC);
	tipMat
		*= Matrix::CreateTranslation(Cudgel::CUDGEL_LENGTH)
		*= CalculateAttackMatrix();
	tip = Vector3::Transform(Vector3::Zero, tipMat);		// ���f���̐�[�̈ʒu���擾

		m_rootPos.push_back(root);							// ���{���W���X�g�̐�[�ɋL�^
		m_tipPos .push_back(tip);							// ���_���W���X�g�̐�[�ɋL�^


	// 2�ȏ�Ȃ��ꍇ�͏����𔲂���
	int max = static_cast<int>(m_rootPos.size() - 1);
	if (max >= 1)
	{
		DirectX::VertexPositionTexture ver[4] =	// ���_���̐����i�p�[�e�B�N���̐����ɕK�v�j
		{
			DirectX::VertexPositionTexture(m_tipPos [max]		,DirectX::SimpleMath::Vector2(0, 0)),	// ����
			DirectX::VertexPositionTexture(m_tipPos [max - 1]	,DirectX::SimpleMath::Vector2(1, 0)),	// �E��
			DirectX::VertexPositionTexture(m_rootPos[max - 1]	,DirectX::SimpleMath::Vector2(1, 1)),	// �E��
			DirectX::VertexPositionTexture(m_rootPos[max]		,DirectX::SimpleMath::Vector2(0, 1)),	// ����
		};

		EventMessenger::Execute(EventList::CreateWeaponTrail, &ver);	// �p�[�e�B�N���̐���
	}
}


// ------------------------
// ��ԍX�V����
// ------------------------
void Cudgel_Sweeping::PostUpdate()
{
}


// ------------------------
// �I������
// ------------------------
void Cudgel_Sweeping::Finalize()
{
}


// ------------------------
// �Փ˔���
// ------------------------
void Cudgel_Sweeping::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}
