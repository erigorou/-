/// ---------------------------
///
/// �v���C���[�̌��̍U���i�~�؂�j
/// 
/// ---------------------------

#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/Collision.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "GeometricPrimitive.h"

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Weapon/Sword/Header/Sword_Attacking_1.h"


// --------------------------------
//  �Œ�l
// --------------------------------
const float Sword_Attacking_1::RADIAN_90 = DirectX::XMConvertToRadians(90);
const float Sword_Attacking_1::ATTACK_TIME  = 0.5f;


// --------------------------------
//  �R���X�g���N�^
// --------------------------------
Sword_Attacking_1::Sword_Attacking_1(Sword* sword)
	:
	m_sword(sword),
	m_position(5.0f, 0.0f, 5.0f),
	m_velocity(0.0f, 0.0f, 0.0f),
	m_angle(0.0f),
	m_rot(0.0f, 0.0f, 0.0f),
	m_totalSeconds(0.0f),
	m_worldMatrix(DirectX::SimpleMath::Matrix::Identity),
	m_model(nullptr)
{
	m_particles = m_sword->GetPlayScene()->GetParticle();
}

// --------------------------------
//  �f�X�g���N�^
// --------------------------------
Sword_Attacking_1::~Sword_Attacking_1()
{
}


// --------------------------------
//  ����������
// --------------------------------
void Sword_Attacking_1::Initialize()
{
	using namespace DirectX::SimpleMath;
	// ���f�����擾
	m_model = m_sword->GetModel();
	// ���[���h�s���������
	m_worldMatrix = Matrix::Identity;
}


// --------------------------------
//  ��ԊJ�n����
// --------------------------------
void Sword_Attacking_1::PreUpdate()
{
	m_totalSeconds = 0.0f;									// �o�ߎ��Ԃ̏�����
	m_sword->GetPlayScene()->GetEnemy()->CanHit(true);		// �Փˉ\�ɂ���

	// �p�[�e�B�N���̏�����
	m_rootPos.clear();
	m_tipPos.clear();
}


// --------------------------------
//  �X�V����
// --------------------------------
void Sword_Attacking_1::Update(float elapsedTime)
{
	using namespace DirectX;
	UNREFERENCED_PARAMETER(elapsedTime);

	// �o�ߎ��Ԃ��v��
	m_totalSeconds += elapsedTime;

	// �v���C���[�̍��W���擾
	m_position = m_sword->GetPlayScene()->GetPlayer()->GetPosition();
	// �v���C���[�̉�]���擾
	m_angle = m_sword->GetPlayScene()->GetPlayer()->GetAngle();



	float t = 0.0f;

	if (m_totalSeconds <= ATTACK_TIME)
	{
		// 0-1�̊Ԃ̒l���擾
		t = m_totalSeconds / ATTACK_TIME;
		m_rot.y = 250.0f * Easying::easeOutBack(t);
		m_rot.x = 10 + 30.0f * Easying::easeOutBack(t);


		m_rot.x = XMConvertToRadians(m_rot.x);
		m_rot.y = XMConvertToRadians(m_rot.y);
	}
	else
	{
		// �U�����Ԃ��߂����瓖���蔻��𖳌��ɂ���
		m_sword->GetPlayScene()->GetEnemy()->CanHit(false);
	}

	// ���[���h�s����X�V����
	m_worldMatrix = Matrix::CreateScale(Sword::SWORD_SCALE);							// ���̃T�C�Y�̐ݒ�

	m_worldMatrix
		*= SimpleMath::Matrix::CreateRotationX(RADIAN_90 / 2)							// ����90�x���Ɍ�����
		*= SimpleMath::Matrix::CreateTranslation(Vector3(1.0f, 2.0f, 0.0f))				// �����������炷
		*= SimpleMath::Matrix::CreateRotationX(m_rot.x)									// �ガ�����̉�]�𔽉f

		*= SimpleMath::Matrix::CreateRotationY(-m_angle)								// �v���C���[�̉��ɉ�]������
		*= SimpleMath::Matrix::CreateRotationY(m_rot.y)									// �ガ�����̉�]�𔽉f
		*= SimpleMath::Matrix::CreateTranslation(m_position);							// �v���C���[�̈ʒu�ɐݒ�

	// �����蔻��̈ʒu��ݒ�
	m_sword->SetCollisionPosition(m_worldMatrix);


	// �U�����I�������X�e�[�g��IdlingState�ɖ߂�
	if (m_totalSeconds >= 1.0f)
	{
		m_sword->ChangeState(m_sword->GetIdlingState());
	}

	// �G�t�F�N�g�`��p�̍��{�ƒ��_��`��
	GetCudgelBothEnds();
}


// --------------------------------
//  ���[���W�̎擾����
// --------------------------------
void Sword_Attacking_1::GetCudgelBothEnds()
{
	// ���{�ƒ��_�̃��[���h���W�����ꂼ��擾
	m_rootPos.	push_back(Vector3::Transform(Vector3(0.0f, Sword::MODEL_ROOT_HEIGHT	, 0.0f), m_worldMatrix));
	m_tipPos.	push_back(Vector3::Transform(Vector3(0.0f, Sword::MODEL_TOP_HEIGHT	, 0.0f), m_worldMatrix));

	// �p�[�e�B�N���𐶐�
	CreateSwordParticle();
}


// --------------------------------
//  �a���G�t�F�N�g��������
// --------------------------------
void Sword_Attacking_1::CreateSwordParticle()
{
	int max = static_cast<int>(m_rootPos.size()) - 1;
	if (max > 1)
	{
		DirectX::VertexPositionTexture ver[4] =
		{
			DirectX::VertexPositionTexture(m_tipPos[max]		,Vector2(0, 0)),	// ����
			DirectX::VertexPositionTexture(m_tipPos[max - 1]	,Vector2(1, 0)),	// �E��
			DirectX::VertexPositionTexture(m_rootPos[max - 1]	,Vector2(1, 1)),	// �E��
			DirectX::VertexPositionTexture(m_rootPos[max]		,Vector2(0, 1)),	// ����
		};
		m_particles->CreateSwordTrail(ver);
	}
}


// --------------------------------
//  ��ԏI������
// --------------------------------
void Sword_Attacking_1::PostUpdate()
{
}


// --------------------------------
//  �`�揈��
// --------------------------------
void Sword_Attacking_1::Render(ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	CommonResources* resources = CommonResources::GetInstance();

	// ���f����`�悷��
	m_model->Draw(context, *states, m_worldMatrix, view, projection);


#ifdef _DEBUG
	auto debugString = resources->GetDebugString();
	debugString->AddString("");
#endif // _DEBUG
}


// --------------------------------
//  �I������
// --------------------------------
void Sword_Attacking_1::Finalize()
{
}


// --------------------------------
//  �Փˎ��C�x���g
// --------------------------------
void Sword_Attacking_1::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}
