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

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Weapon/Cudgel/Header/Cudgel_Sweeping.h"
#include "Game/Weapon/Cudgel/Header/Cudgel.h"


// �Œ�l ==================================
const float Cudgel_Sweeping::CHARGE_TIME = 1.0f;	// �U�肩��������
const float Cudgel_Sweeping::WINDUP_TIME = 1.3f;	// �ҋ@
const float Cudgel_Sweeping::ATTACK_TIME = 1.8f;	// �ガ����
const float Cudgel_Sweeping::END_TIME	 = 2.5f;	// �I��

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
	, m_model(nullptr)
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
	m_model = m_cudgel->GetModel();
	m_originalBox = Collision::Get_BoundingOrientedBox_FromMODEL(m_model);	// ���f������傫�����擾
}


// ���O����
void Cudgel_Sweeping::PreUpdate()
{
	// �o�ߎ��Ԃ̏�����
	m_totalSeconds = 0.0f;												// �o�ߎ��Ԃ̏�����
	m_angleUD = DirectX::XMConvertToRadians(90.0f);						// ���_�����ɂ���
	m_angleRL = 0.0f;													// ����]�̏�����
	m_parentAngleRL = m_cudgel->GetPlayScene()->GetEnemy()->GetAngle();	// �G�̊p�x���擾�i�e�̊p�x�j

	// ���_���̏�����
	m_rootPos.clear();
	m_tipPos.clear();

	m_canGenerateSlamParticles = true;
}



// �X�V����
void Cudgel_Sweeping::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;

	// ���v���Ԃ��v��
	m_totalSeconds		+= elapsedTime;

	auto enemy = m_cudgel->GetPlayScene()->GetEnemy();
	m_position = enemy->GetPosition();	// �G�̍��W���擾

	UpdateCudgelRotation();				// ��]���v�Z����
	CalculateModelMatrix();				// ���[���h�s����v�Z
	GetCudgelBothEnds(m_totalSeconds);	// ���[���擾����

	m_originalBox.Transform(m_boundingBox, m_worldMatrix);	// �����蔻����ړ�������@��
}


/// <summary>
/// Cudgel�̏c���̉�]�̍X�V�֐�
/// </summary>
void Cudgel_Sweeping::UpdateCudgelRotation()
{
	float t = 0.0f;  // ���K�����ꂽ�o�ߎ���

	// ---- �������̉�]�i���E�̉�]�j----
	if (m_totalSeconds <= CHARGE_TIME) {
				t = m_totalSeconds / CHARGE_TIME;  // 0 ~ 1 �ɐ��K��
		m_angleRL = -CHARGE_ROTATE_ANGLE * m_easying->easeOutCirc(t);  // 30�x����]
	}

	else if (m_totalSeconds >= WINDUP_TIME && m_totalSeconds <= ATTACK_TIME) {
		t = (m_totalSeconds - WINDUP_TIME) / (ATTACK_TIME - WINDUP_TIME);  // 0 ~ 1 �ɐ��K��
		m_angleRL = CHARGE_ROTATE_ANGLE + WINDUP_ROTATE_ANGLE * m_easying->easeOutBack(t);  // 30�x����60�x�E��]
	}
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
		*=  Matrix::CreateRotationY(DirectX::XMConvertToRadians(-m_angleRL) - m_parentAngleRL - DirectX::XMConvertToRadians(189))	// ����]���s��
		*=  Matrix::CreateTranslation(Cudgel_Sweeping::ARM_LENGTH)																	// �r�̒������ړ�
		*=  Matrix::CreateTranslation(m_position);																					// �Ō�ɓG�̈ʒu�ɐݒ�
}


/// <summary>
/// �G�t�F�N�g�ȂǂɎg�p������_�̍��{�ƒ��_�̍��W���擾����֐�
/// </summary>
/// <param name="_elapsedTime">�o�ߎ���</param>
void Cudgel_Sweeping::GetCudgelBothEnds(float _totalTime)
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

		m_rootPos.push_back(root);		// ���{���W���X�g�̐�[�ɋL�^
		m_tipPos .push_back(tip);		// ���_���W���X�g�̐�[�ɋL�^

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


/// <summary>
/// ���㏈��
/// </summary>
void Cudgel_Sweeping::PostUpdate()
{
}


/// <summary>
/// �`�揈��
/// </summary>
/// <param name="context"></param>
/// <param name="states"></param>
/// <param name="view"></param>
/// <param name="projection"></param>
void Cudgel_Sweeping::Render(ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	// ���f����`�悷��
	m_model->Draw(context, *states, m_worldMatrix, view, projection);

#ifdef _DEBUG
	CommonResources* resources = CommonResources::GetInstance();
	auto debugString = resources->GetDebugString();
	debugString->AddString("%f", m_angleRL);

	// Sphere�̐���
	//auto sphere = DirectX::GeometricPrimitive::CreateSphere(context, 1.0f);

	// m_rootPos�̊e���_�ɋ��̂�`��
	//for (const auto& rootPos : m_rootPos)
	//{
	//	DirectX::SimpleMath::Matrix rootSphereMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(rootPos);
	//	sphere->Draw(rootSphereMatrix, view, projection, DirectX::Colors::Red, nullptr, true);
	//}

	// m_tipPos�̊e���_�ɋ��̂�`��
	//for (const auto& tipPos : m_tipPos)
	//{
	//	DirectX::SimpleMath::Matrix tipSphereMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(tipPos);
	//	sphere->Draw(tipSphereMatrix, view, projection, DirectX::Colors::Blue, nullptr, true);
	//}
#endif // _DEBUG
}


// �I������
void Cudgel_Sweeping::Finalize()
{
}