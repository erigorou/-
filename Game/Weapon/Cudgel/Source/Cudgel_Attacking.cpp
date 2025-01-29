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
#include "Game/Messenger/EventMessenger.h"

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Weapon/Cudgel/Header/Cudgel_Attacking.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Sound/Sound.h"


// �Œ�l ==================================
const float Cudgel_Attacking::CHARGE_TIME = 0.8f;	// �U�肩��������
const float Cudgel_Attacking::WINDUP_TIME = 0.9f;	// �ҋ@
const float Cudgel_Attacking::ATTACK_TIME = 1.5f;	// �~�艺�낵
const float Cudgel_Attacking::STOP_TIME	  = 2.3f;	// �ҋ@
const float Cudgel_Attacking::RETURN_TIME = 2.8f;	// ���ɖ߂�
const float Cudgel_Attacking::END_TIME    = 3.3f;	// �I������

const Vector3 Cudgel_Attacking::ARM_LENGTH = Vector3(0.0f, 4.0f, 0.0f);
const Vector3 Cudgel_Attacking::ZERO_DIREC = Vector3(8.0f, 1.0f, 0.0f);


// �R���X�g���N�^
Cudgel_Attacking::Cudgel_Attacking(Cudgel* cudgel)
	:m_cudgel(cudgel)
	, m_position(0.0f, 0.0f, 0.0f)
	, m_velocity(0.0f, 0.0f, 0.0f)
	, m_angleRL(0.0f)
	, m_angleUD(0.0f)
	, m_totalSeconds(0.0f)
	, m_recordPointTimer(0.0f)
	, m_worldMatrix(DirectX::SimpleMath::Matrix::Identity)
	, m_canGenerateSlamParticles(true)
{
	// �p�[�e�B�N���̎擾
	m_particles = cudgel->GetPlayScene()->GetParticle();
}

// �f�X�g���N�^
Cudgel_Attacking::~Cudgel_Attacking()
{
}


// ����������
void Cudgel_Attacking::Initialize()
{
	m_worldMatrix = Matrix::Identity;			// ���[���h�s��̏�����
}


// ���O����
void Cudgel_Attacking::PreUpdate()
{
	// �o�ߎ��Ԃ̏�����
	m_totalSeconds = 0.0f;
	m_angleUD = 0.0f;
	m_canHit = false;
	m_recordPointTimer = 0.0f;

	// ���_���̏�����
	m_rootPos.clear();
	m_tipPos.clear();

	// �p�[�e�B�N���𐶐��\��
	m_canGenerateSlamParticles = true;
	// �����T�E���h���\��
	m_playSound = false;
}



// �X�V����
void Cudgel_Attacking::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;

	// ���v���Ԃ��v��
	m_totalSeconds		+= elapsedTime;
	m_recordPointTimer	+= elapsedTime;

	auto enemy = m_cudgel->GetPlayScene()->GetEnemy();
	m_position = enemy->GetPosition();	// �G�̍��W���擾
	m_enemyAngle = enemy->GetAngle();	// �G�̉�]���擾
	m_angleRL = m_enemyAngle;			// �G�̉�]��ݒ�

	UpdateCudgelRotation();				// ��]���v�Z����
	CalculateModelMatrix();				// ���[���h�s����v�Z
	GetCudgelBothEnds(m_totalSeconds);	// ���[���擾����

	m_cudgel->SetWorldMatrix(m_worldMatrix);		// ���[���h�s���ݒ肷��
	m_cudgel->SetCollisionPosition(m_worldMatrix);	// �����蔻��̈ʒu��ݒ肷��
}


/// <summary>
/// Cudgel�̏c���̉�]�̍X�V�֐�
/// </summary>
void Cudgel_Attacking::UpdateCudgelRotation()
{
	UpdateAttackState();	// �U����Ԃ̍X�V
}


/// <summary>
/// �U�肩�����̏����B�����20�x�����グ��B
/// </summary>
/// <param name="t">0����1�ɐ��K�����ꂽ���ԁB</param>
void Cudgel_Attacking::HandleChargePhase(float t)
{
	// 20�x�グ��i0.5�b�ԂŁA�C�[�W���O�g�p�j
	m_angleUD = DirectX::XMConvertToRadians(-40.0f * Easing::easeInOutBack(t));
}


/// <summary>
/// �ҋ@��Ԃ̏����B�����20�x�̊p�x�ŕێ�����B
/// </summary>
void Cudgel_Attacking::HandleWindoupPhase()
{
	m_canHit = true;

	// �U�肩�����̊p�x��ێ��i-40�x�̏�Ԃ��ێ��j
	m_angleUD = DirectX::XMConvertToRadians(-40.0f);
}


/// <summary>
/// �U���t�F�[�Y�̏����B�����20�x����95�x�܂ŐU�艺�낷�B
/// </summary>
/// <param name="t">0����1�ɐ��K�����ꂽ���ԁB</param>
void Cudgel_Attacking::HandleAttackPhase(float t)
{
	// 20�x����115�x�U�艺�낷�i0.3�b�ԂŁA�C�[�W���O�g�p�j
	m_angleUD = DirectX::XMConvertToRadians(-40.0f + 135.0f * Easing::easeInQuint(t));
	m_angleRL = DirectX::XMConvertToRadians(-ATTACK_ANGLE_UD * Easing::easeInQuint(t)) + m_angleRL;

	if (t > 0.9f && m_playSound == false)
	{
		Sound::PlaySE(Sound::SE_TYPE::ENEMY_ATTACK);
		m_playSound = true;
	}
}

/// <summary>
/// ���������Ă���ҋ@���郂�[�V����
/// </summary>
void Cudgel_Attacking::KeepStampPhase()
{
	m_canHit = false;

	m_angleUD = DirectX::XMConvertToRadians(95.0f);
	m_angleRL = DirectX::XMConvertToRadians(-ATTACK_ANGLE_UD) + m_angleRL;

	// �����������u�ԂɃp�[�e�B�N���̐���
	HandleSlamParticles();
}


/// <summary>
/// �ҋ@��Ԃɖ߂鏈���B
/// </summary>
/// <param name="t"></param>
void Cudgel_Attacking::ReturnToOriginalPhase(float t)
{

	// ���Əc�̉�]�����ɖ߂�
	m_angleUD = DirectX::XMConvertToRadians(95.0f - 95.0f * t);
	m_angleRL = DirectX::XMConvertToRadians(- ATTACK_ANGLE_UD + ATTACK_ANGLE_UD * t) + m_angleRL;
}

/// <summary>
/// �㌄�̃p�[�e�B�N�����������B����̐�[�ɐo�𔭐������A�J������U��������B
/// </summary>
void Cudgel_Attacking::HandleSlamParticles()
{
	if (m_canGenerateSlamParticles)
	{
		m_canGenerateSlamParticles = false;
		m_cudgel->GetPlayScene()->GetParticle()->CreateSlamDust(&(m_tipPos[m_tipPos.size() - 1]));

		float m_shakePower = 1.5f;

		// �J�����̐U����ݒ�
		EventMessenger::Execute("CameraShake", &m_shakePower);
	}
}



/// <summary>
/// �X�V���鏈��
/// </summary>
void Cudgel_Attacking::UpdateAttackState()
{
	if		(m_totalSeconds < CHARGE_TIME)									HandleChargePhase(m_totalSeconds / CHARGE_TIME);
	else if (m_totalSeconds > CHARGE_TIME && m_totalSeconds <= WINDUP_TIME)	HandleWindoupPhase();
	else if (m_totalSeconds > WINDUP_TIME && m_totalSeconds <= ATTACK_TIME)	HandleAttackPhase((m_totalSeconds - WINDUP_TIME) / (ATTACK_TIME - WINDUP_TIME));
	else if (m_totalSeconds > ATTACK_TIME && m_totalSeconds <= STOP_TIME)	KeepStampPhase();
	else if (m_totalSeconds > STOP_TIME && m_totalSeconds <= RETURN_TIME)	ReturnToOriginalPhase((m_totalSeconds - STOP_TIME) / (RETURN_TIME - STOP_TIME));

	// �v���C���[�ɍU���\��Ԃ�ʒm
	m_cudgel->GetPlayScene()->GetPlayer()->CanHit(m_canHit);
}




/// <summary>
/// �`��p���[���h�s����v�Z����֐�
/// </summary>
void Cudgel_Attacking::CalculateModelMatrix()
{
	m_worldMatrix = Matrix::CreateScale(Cudgel::CUDGEL_SCALE);		// �T�C�Y�̐ݒ� & ������
	m_worldMatrix
		*= Matrix::CreateTranslation(Cudgel_Attacking::ZERO_DIREC)	// �����ʒu�ւ̈ړ�
		*= CalculateAttackMatrix();									// �U�����[�V�������̌v�Z
}


/// <summary>
/// Cudgel�̃��[���h�s����v�Z����֐�
/// </summary>
/// <returns>�U�����[�V��������Cudgel�̉�]���[���h�s��</returns>
DirectX::SimpleMath::Matrix Cudgel_Attacking::CalculateAttackMatrix()
{
	return 
			Matrix::CreateRotationX(-m_angleUD)							// �c��]���s��
		*=  Matrix::CreateRotationY(-m_angleRL)							// ����]���s��
		*=  Matrix::CreateTranslation(Cudgel_Attacking::ARM_LENGTH)		// �r�̒������ړ�
		*=  Matrix::CreateTranslation(m_position);						// �Ō�ɓG�̈ʒu�ɐݒ�
}


/// <summary>
/// �G�t�F�N�g�ȂǂɎg�p������_�̍��{�ƒ��_�̍��W���擾����֐�
/// </summary>
/// <param name="_elapsedTime">�o�ߎ���</param>
void Cudgel_Attacking::GetCudgelBothEnds(float _totalTime)
{
	UNREFERENCED_PARAMETER(_totalTime);

	DirectX::SimpleMath::Vector3 root;
	DirectX::SimpleMath::Vector3 tip;

	Matrix rootMat = Matrix::CreateTranslation(Cudgel_Attacking::ZERO_DIREC);
	rootMat
		*= Matrix::CreateTranslation(Cudgel::CUDGEL_HADLE_POS)
		*= CalculateAttackMatrix();
	root = Vector3::Transform(Vector3::Zero, rootMat);		// ���f���̐�[�̈ʒu���擾

	
	Matrix tipMat = Matrix::CreateTranslation(Cudgel_Attacking::ZERO_DIREC);
	tipMat
		*= Matrix::CreateTranslation(Cudgel::CUDGEL_LENGTH)
		*= CalculateAttackMatrix();
	tip = Vector3::Transform(Vector3::Zero, tipMat);		// ���f���̐�[�̈ʒu���擾

	m_recordPointTimer = 0.0f;
	m_rootPos.push_back(root);		// ���{���W���X�g�̐�[�ɋL�^
	m_tipPos .push_back(tip);		// ���_���W���X�g�̐�[�ɋL�^

	using namespace DirectX;

	// 2�ȏ�Ȃ��ꍇ�͏����𔲂���
	size_t max = m_rootPos.size() - 1;
	if (max >= 1)
	{
		VertexPositionTexture ver[4] =	// ���_���̐����i�p�[�e�B�N���̐����ɕK�v�j
		{
			VertexPositionTexture(m_tipPos [max]		,Vector2(0, 0)),	// ����
			VertexPositionTexture(m_tipPos [max - 1]	,Vector2(1, 0)),	// �E��
			VertexPositionTexture(m_rootPos[max - 1]	,Vector2(1, 1)),	// �E��
			VertexPositionTexture(m_rootPos[max]		,Vector2(0, 1)),	// ����
		};

		m_particles->CreateSwordTrail(ver);	// �p�[�e�B�N���̐���
	}
}


/// <summary>
/// ���㏈��
/// </summary>
void Cudgel_Attacking::PostUpdate()
{
}


// �I������
void Cudgel_Attacking::Finalize()
{
}


void Cudgel_Attacking::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}
