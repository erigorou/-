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

#include "Game/Player/Player.h"
#include "Game/Boss/Boss.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Weapon/Cudgel/Header/Cudgel_Sweeping.h"
#include "Game/Weapon/Cudgel/Cudgel.h"


// �Œ�l ==================================
const float Cudgel_Sweeping::CHARGE_TIME = 0.8f;	// �U�肩��������
const float Cudgel_Sweeping::WINDUP_TIME = 0.9f;	// �ҋ@
const float Cudgel_Sweeping::ATTACK_TIME = 1.5f;	// �ガ����
const float Cudgel_Sweeping::END_TIME	 = 2.3f;	// �I��

const float Cudgel_Sweeping::CHARGE_ROTATE_ANGLE = 30.0f;	// �U�肩�����p�x
const float Cudgel_Sweeping::WINDUP_ROTATE_ANGLE = 240.0f;	// �ガ�����p�x

const Vector3 Cudgel_Sweeping::ARM_LENGTH = Vector3(0.0f, 5.0f, 0.0f);
const Vector3 Cudgel_Sweeping::ZERO_DIREC = Vector3(6.0f, 1.0f, 0.0f);


// �R���X�g���N�^
Cudgel_Sweeping::Cudgel_Sweeping(Cudgel* cudgel)
	:m_cudgel(cudgel)
	, m_position(0.0f, 0.0f, 0.0f)
	, m_velocity(0.0f, 0.0f, 0.0f)
	, m_angleRL(0.0f)
	, m_angleUD(0.0f)
	, m_parentAngleRL(0.0f)
	, m_totalSeconds(0.0f)
	, m_worldMatrix(DirectX::SimpleMath::Matrix::Identity)
	, m_canGenerateSlamParticles(true)
{
	// �p�[�e�B�N���̎擾
	m_particles = cudgel->GetPlayScene()->GetParticle();
}

// �f�X�g���N�^
Cudgel_Sweeping::~Cudgel_Sweeping()
{
}


// ����������
void Cudgel_Sweeping::Initialize()
{
	m_worldMatrix = Matrix::Identity;			// ���[���h�s��̏�����
}


// ���O����
void Cudgel_Sweeping::PreUpdate()
{
	m_totalSeconds = 0.0f;												// �o�ߎ��Ԃ̏�����
	m_angleUD = DirectX::XMConvertToRadians(93.0f);						// ���_�����ɂ���
	m_angleRL = 0.0f;													// ����]�̏�����
	m_parentAngleRL = m_cudgel->GetPlayScene()->GetBoss()->GetAngle();	// �G�̊p�x���擾�i�e�̊p�x�j
	m_rootPos.clear();
	m_tipPos.clear();

	m_canGenerateSlamParticles = true;		// �G�t�F�N�g�����t���O��true�ɂ���
	m_playSound = false;					// �T�E���h�̍Đ��t���O��false�ɂ���
}



// �X�V����
void Cudgel_Sweeping::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;

	// ���v���Ԃ��v��
	m_totalSeconds += elapsedTime;

	auto enemy = m_cudgel->GetPlayScene()->GetBoss();
	m_position = enemy->GetPosition();	// �G�̍��W���擾

	UpdateCudgelRotation();		// ��]���v�Z����
	CalculateModelMatrix();		// ���[���h�s����v�Z
	GetCudgelBothEnds();		// ���[���擾����


	m_cudgel->SetWorldMatrix(m_worldMatrix);		// ���[���h�s���ݒ肷��
	m_collMatrix = m_worldMatrix;					// �����蔻��p�̍s����擾
	m_collMatrix._42 = 0.0f;						// �����蔻��̍�����0�ɂ���
	m_cudgel->SetCollisionPosition(m_collMatrix);	// �����蔻��̈ʒu��ݒ肷��
}


/// <summary>
/// Cudgel�̏c���̉�]�̍X�V�֐�
/// </summary>
void Cudgel_Sweeping::UpdateCudgelRotation()
{
	bool canHit = false;
	float t = 0.0f;  // ���K�����ꂽ�o�ߎ���

	// ���߃��[�V����
	if (m_totalSeconds <= CHARGE_TIME) {
		t =	m_totalSeconds / CHARGE_TIME;							// 0 ~ 1 �ɐ��K��
		m_angleRL = -CHARGE_ROTATE_ANGLE * Easing::easeOutCirc(t);	// 30�x����]
	}

	// �U�����[�V����
	else if (m_totalSeconds >= WINDUP_TIME && m_totalSeconds <= ATTACK_TIME) {
		t = (m_totalSeconds - WINDUP_TIME) / (ATTACK_TIME - WINDUP_TIME);					// 0 ~ 1 �ɐ��K��
		m_angleRL = CHARGE_ROTATE_ANGLE + WINDUP_ROTATE_ANGLE * Easing::easeOutBack(t);  // 30�x����60�x�E��]
		canHit = true;

		// SE�̍Đ�
		if (m_playSound == false)
		{
			Sound::PlaySE(Sound::SE_TYPE::BOSS_SWEEP);
			m_playSound = true;
		}
	}

	else if (m_totalSeconds > END_TIME) {
		m_cudgel->ChangeState(m_cudgel->GetIdling());
	}

	m_cudgel->GetPlayScene()->GetPlayer()->CanHit(canHit);
}


/// <summary>
/// �`��p���[���h�s����v�Z����֐�
/// </summary>
void Cudgel_Sweeping::CalculateModelMatrix()
{
	m_worldMatrix = Matrix::CreateScale(Cudgel::CUDGEL_SCALE);		// �T�C�Y�̐ݒ� & ������
	m_worldMatrix
		*= Matrix::CreateTranslation(Cudgel_Sweeping::ZERO_DIREC)	// �����ʒu�ւ̈ړ�
		*= CalculateAttackMatrix();									// �U�����[�V�������̌v�Z
}


/// <summary>
/// Cudgel�̃��[���h�s����v�Z����֐�
/// </summary>
/// <returns>�U�����[�V��������Cudgel�̉�]���[���h�s��</returns>
DirectX::SimpleMath::Matrix Cudgel_Sweeping::CalculateAttackMatrix()
{
	return 
			Matrix::CreateRotationX(-m_angleUD)																						// �c��]���s��
		*=  Matrix::CreateRotationY(DirectX::XMConvertToRadians(-m_angleRL) - m_parentAngleRL - DirectX::XMConvertToRadians(180))	// ����]���s��
		*=  Matrix::CreateTranslation(Cudgel_Sweeping::ARM_LENGTH)																	// �r�̒������ړ�
		*=  Matrix::CreateTranslation(m_position);																					// �Ō�ɓG�̈ʒu�ɐݒ�
}


/// <summary>
/// �G�t�F�N�g�ȂǂɎg�p������_�̍��{�ƒ��_�̍��W���擾����֐�
/// </summary>
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

	using namespace DirectX;



	// 2�ȏ�Ȃ��ꍇ�͏����𔲂���
	int max = static_cast<int>(m_rootPos.size() - 1);
	if (max >= 1)
	{
		VertexPositionTexture ver[4] =	// ���_���̐����i�p�[�e�B�N���̐����ɕK�v�j
		{
			VertexPositionTexture(m_tipPos [max]		,Vector2(0, 0)),	// ����
			VertexPositionTexture(m_tipPos [max - 1]	,Vector2(1, 0)),	// �E��
			VertexPositionTexture(m_rootPos[max - 1]	,Vector2(1, 1)),	// �E��
			VertexPositionTexture(m_rootPos[max]		,Vector2(0, 1)),	// ����
		};

		EventMessenger::Execute("CreateSwordTrail", &ver);	// �p�[�e�B�N���̐���
	}
}


/// <summary>
/// ���㏈��
/// </summary>
void Cudgel_Sweeping::PostUpdate()
{
}


// �I������
void Cudgel_Sweeping::Finalize()
{
}

void Cudgel_Sweeping::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}
