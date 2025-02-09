/// ---------------------------
///
/// �S�̋��_�̍U�����
///
/// ---------------------------

#include "pch.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/Player/Player.h"
#include "Game/Boss/Boss.h"
#include "Game/Weapon/Cudgel/Header/Cudgel_Attacking.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Sound/Sound.h"

// �Œ�l ==================================
const float Cudgel_Attacking::CHARGE_TIME = 0.8f;	// �U�肩��������
const float Cudgel_Attacking::WINDUP_TIME = 0.9f;	// �ҋ@
const float Cudgel_Attacking::ATTACK_TIME = 1.5f;	// �~�艺�낵
const float Cudgel_Attacking::STOP_TIME = 2.3f;	// �ҋ@
const float Cudgel_Attacking::RETURN_TIME = 2.8f;	// ���ɖ߂�
const float Cudgel_Attacking::END_TIME = 3.3f;	// �I������

const Vector3 Cudgel_Attacking::ARM_LENGTH = Vector3(0.0f, 4.0f, 0.0f);
const Vector3 Cudgel_Attacking::ZERO_DIREC = Vector3(8.0f, 1.0f, 0.0f);

// �R���X�g���N�^
Cudgel_Attacking::Cudgel_Attacking(Cudgel* cudgel)
	:
	m_cudgel(cudgel),
	m_position{},
	m_velocity{},
	m_angleRL{},
	m_angleUD{},
	m_totalSeconds{},
	m_recordPointTimer{},
	m_worldMatrix(DirectX::SimpleMath::Matrix::Identity),
	m_canGenerateSlamParticles(true)
{
}

// �f�X�g���N�^
Cudgel_Attacking::~Cudgel_Attacking()
{
}

// ����������
void Cudgel_Attacking::Initialize()
{
	// ���[���h�s��̏�����
	m_worldMatrix = Matrix::Identity;
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
	// ���v���Ԃ��v��
	m_totalSeconds += elapsedTime;
	m_recordPointTimer += elapsedTime;

	auto boss = m_cudgel->GetBoss();
	m_position = boss->GetPosition();	// �G�̍��W���擾
	m_enemyAngle = boss->GetAngle();	// �G�̉�]���擾
	m_angleRL = m_enemyAngle;			// �G�̉�]��ݒ�

	UpdateAnimation();	// �A�j���[�V�����̍X�V
}

void Cudgel_Attacking::UpdateAnimation()
{
	// ���_�̉�]���X�V
	UpdateCudgelRotation();
	// ���_�̃��[���h�s����v�Z
	CalculateModelMatrix();
	// ���_�̍��{�Ɛ�[�̍��W���擾
	GetCudgelBothEnds(m_totalSeconds);

	// �v�Z�������[���h�s���ݒ�
	m_cudgel->SetWorldMatrix(m_worldMatrix);
	m_cudgel->SetCollisionPosition(m_worldMatrix);
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
	// �U�肩�����̊p�x��ێ��i-40�x�̏�Ԃ��ێ��j
	m_angleUD = DirectX::XMConvertToRadians(-40.0f);
}

/// <summary>
/// �U���t�F�[�Y�̏����B�����20�x����95�x�܂ŐU�艺�낷�B
/// </summary>
/// <param name="t">0����1�ɐ��K�����ꂽ���ԁB</param>
void Cudgel_Attacking::HandleAttackPhase(float t)
{
	m_canHit = true;

	// 20�x����115�x�U�艺�낷�i0.3�b�ԂŁA�C�[�W���O�g�p�j
	m_angleUD = DirectX::XMConvertToRadians(-40.0f + 135.0f * Easing::easeInQuint(t));
	m_angleRL = DirectX::XMConvertToRadians(-ATTACK_ANGLE_UD * Easing::easeInQuint(t)) + m_angleRL;

	if (t > 0.9f && m_playSound == false)
	{
		Sound::PlaySE(Sound::SE_TYPE::BOSS_ATTACK);
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
	m_angleRL = DirectX::XMConvertToRadians(-ATTACK_ANGLE_UD + ATTACK_ANGLE_UD * t) + m_angleRL;
}

/// <summary>
/// �㌄�̃p�[�e�B�N�����������B����̐�[�ɐo�𔭐������A�J������U��������B
/// </summary>
void Cudgel_Attacking::HandleSlamParticles()
{
	if (m_canGenerateSlamParticles)
	{
		m_canGenerateSlamParticles = false;

		// �p�[�e�B�N���̐���
		EventMessenger::Execute(EventList::CreateBashDust, (&m_tipPos[m_tipPos.size() - 1]));

		// �J�����̐U����ݒ�
		float m_shakePower = 1.5f;
		EventMessenger::Execute(EventList::ShakeCamera, &m_shakePower);
	}
}

/// <summary>
/// �X�V���鏈��
/// </summary>
void Cudgel_Attacking::UpdateAttackState()
{
	if (m_totalSeconds < CHARGE_TIME)									HandleChargePhase(m_totalSeconds / CHARGE_TIME);
	else if (m_totalSeconds > CHARGE_TIME && m_totalSeconds <= WINDUP_TIME)	HandleWindoupPhase();
	else if (m_totalSeconds > WINDUP_TIME && m_totalSeconds <= ATTACK_TIME)	HandleAttackPhase((m_totalSeconds - WINDUP_TIME) / (ATTACK_TIME - WINDUP_TIME));
	else if (m_totalSeconds > ATTACK_TIME && m_totalSeconds <= STOP_TIME)	KeepStampPhase();
	else if (m_totalSeconds > STOP_TIME && m_totalSeconds <= RETURN_TIME)	ReturnToOriginalPhase((m_totalSeconds - STOP_TIME) / (RETURN_TIME - STOP_TIME));

	// �v���C���[�ɍU���\����ʒm
	EventMessenger::Execute(EventList::PlayerCanDamageCudgel, &m_canHit);
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
		*= Matrix::CreateRotationY(-m_angleRL)							// ����]���s��
		*= Matrix::CreateTranslation(Cudgel_Attacking::ARM_LENGTH)		// �r�̒������ړ�
		*= Matrix::CreateTranslation(m_position);						// �Ō�ɓG�̈ʒu�ɐݒ�
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
	m_tipPos.push_back(tip);		// ���_���W���X�g�̐�[�ɋL�^

	using namespace DirectX;

	// 2�ȏ�Ȃ��ꍇ�͏����𔲂���
	int max = static_cast<int>(m_rootPos.size()) - 1;
	if (max >= 1)
	{
		VertexPositionTexture ver[4] =	// ���_���̐����i�p�[�e�B�N���̐����ɕK�v�j
		{
			VertexPositionTexture(m_tipPos[max]		,Vector2(0, 0)),	// ����
			VertexPositionTexture(m_tipPos[max - 1]	,Vector2(1, 0)),	// �E��
			VertexPositionTexture(m_rootPos[max - 1]	,Vector2(1, 1)),	// �E��
			VertexPositionTexture(m_rootPos[max]		,Vector2(0, 1)),	// ����
		};

		EventMessenger::Execute(EventList::CreateWeaponTrail, &ver);	// �p�[�e�B�N���̐���
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