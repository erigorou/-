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

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Weapon/Cudgel/Header/Cudgel_Attacking.h"
#include "Game/Weapon/Cudgel/Header/Cudgel.h"


// �Œ�l ==================================
const float Cudgel_Attacking::CHARGE_TIME = 1.0f;	// �U�肩��������
const float Cudgel_Attacking::WINDUP_TIME = 1.4f;	// �ҋ@
const float Cudgel_Attacking::ATTACK_TIME = 1.7f;	// �~�艺�낵

const Vector3 Cudgel_Attacking::ARM_LENGTH = Vector3(0.0f, 4.0f, 0.0f);
const Vector3 Cudgel_Attacking::ZERO_DIREC = Vector3(6.0f, 1.0f, 0.0f);


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
	, m_model(nullptr)
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
	m_model = m_cudgel->GetModel();
	m_originalBox = Collision::Get_BoundingOrientedBox_FromMODEL(m_model);	// ���f������傫�����擾
}


// ���O����
void Cudgel_Attacking::PreUpdate()
{
	// �o�ߎ��Ԃ̏�����
	m_totalSeconds = 0.0f;
	m_angleUD = 0.0f;
	m_recordPointTimer = 0.0f;

	// ���_���̏�����
	m_rootPos.clear();
	m_tipPos.clear();
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
	m_angleRL = enemy->GetAngle();		// �G�̉�]���擾

	UpdateCudgelRotation();				// ��]���v�Z����
	CalculateModelMatrix();				// ���[���h�s����v�Z
	GetCudgelBothEnds(m_totalSeconds);	// ���[���擾����

	m_originalBox.Transform(m_boundingBox, m_worldMatrix);	// �����蔻����ړ�������@��
}


/// <summary>
/// Cudgel�̏c���̉�]�̍X�V�֐�
/// </summary>
void Cudgel_Attacking::UpdateCudgelRotation()
{
	// -----------------------------------------------------------------
	// �G�̍U���̗���
	// �U�肩�����i1�b�j�� �ҋ@�i0.4�b�j�� �~�艺�낷(0.3�b) �� �㌄

	if (m_totalSeconds < CHARGE_TIME)
		m_angleUD = DirectX::XMConvertToRadians(-20.0f * (m_totalSeconds / 0.5f));// 20�x�グ��

	else if (m_totalSeconds > WINDUP_TIME && m_totalSeconds < ATTACK_TIME)
		m_angleUD = DirectX::XMConvertToRadians(-20.0f + 115.0f * ((m_totalSeconds - 1.4f) / 0.3f));// 95�x�U�艺�낷
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
	Matrix rootMat = Matrix::CreateTranslation(Cudgel_Attacking::ZERO_DIREC);
	rootMat
		*= Matrix::CreateTranslation(Cudgel::CUDGEL_HADLE_POS)
		*= CalculateAttackMatrix();
	m_rootDeb = Vector3::Transform(Vector3::Zero, rootMat);		// ���f���̐�[�̈ʒu���擾


	Matrix tipMat = Matrix::CreateTranslation(Cudgel_Attacking::ZERO_DIREC);
	tipMat
		*= Matrix::CreateTranslation(Cudgel::CUDGEL_LENGTH)
		*= CalculateAttackMatrix();
	m_tipDeb = Vector3::Transform(Vector3::Zero, tipMat);		// ���f���̐�[�̈ʒu���擾

	if (m_recordPointTimer >= 0.025f)
	{
		m_recordPointTimer = 0.0f;
		m_rootPos.push_back(m_rootDeb);		// ���{���W���X�g�̐�[�ɋL�^
		m_tipPos .push_back(m_tipDeb);		// ���_���W���X�g�̐�[�ɋL�^

		using namespace DirectX;

		// 2�ȏ�Ȃ��ꍇ�͏����𔲂���
		float max = m_rootPos.size() - 1;
		if (max >= 1)
		{
			VertexPositionTexture ver[4] =	// ���_���̐����i�p�[�e�B�N���̐����ɕK�v�j
			{
				VertexPositionTexture(m_tipPos [max]		,Vector2(0, 0)),	// ����
				VertexPositionTexture(m_tipPos [max - 1]	,Vector2(1, 0)),	// �E��
				VertexPositionTexture(m_rootPos[max - 1]	,Vector2(1, 1)),	// �E��
				VertexPositionTexture(m_rootPos[max]		,Vector2(0, 1)),	// ����
			};

			m_particles->CreateSwordTrial(ver);	// �p�[�e�B�N���̐���
		}
	}
}


/// <summary>
/// ���㏈��
/// </summary>
void Cudgel_Attacking::PostUpdate()
{
}


/// <summary>
/// �`�揈��
/// </summary>
/// <param name="context"></param>
/// <param name="states"></param>
/// <param name="view"></param>
/// <param name="projection"></param>
void Cudgel_Attacking::Render(ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	// ���f����`�悷��
	m_model->Draw(context, *states, m_worldMatrix, view, projection);

	// �p�[�e�B�N���̐���
	if (m_rootPos.max_size() >= 2 && m_tipPos.max_size() >= 2)
	{
		// ���_�����ɕK�v�Ȑ���������Ă���ꍇ
		// ���ꂼ��1 2�@�Ԃ�p���ăp�[�e�B�N���𐶐�����
	}


#ifdef _DEBUG
	CommonResources* resources = CommonResources::GetInstance();
	auto debugString = resources->GetDebugString();
#endif // _DEBUG


	// Sphere�̐���
	auto sphere = DirectX::GeometricPrimitive::CreateSphere(context, 1.0f);

	// m_rootPos�̊e���_�ɋ��̂�`��
	for (const auto& rootPos : m_rootPos)
	{
		DirectX::SimpleMath::Matrix rootSphereMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(rootPos);
		sphere->Draw(rootSphereMatrix, view, projection, DirectX::Colors::Red, nullptr, true);
	}

	// m_tipPos�̊e���_�ɋ��̂�`��
	for (const auto& tipPos : m_tipPos)
	{
		DirectX::SimpleMath::Matrix tipSphereMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(tipPos);
		sphere->Draw(tipSphereMatrix, view, projection, DirectX::Colors::Blue, nullptr, true);
	}
}


// �I������
void Cudgel_Attacking::Finalize()
{
}