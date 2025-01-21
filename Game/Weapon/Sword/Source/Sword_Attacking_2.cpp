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
#include "Game/Weapon/Sword/Header/Sword_Attacking_2.h"


// --------------------------------
//  �Œ�l
// --------------------------------
// 
const float Sword_Attacking_2::RADIAN_90 = DirectX::XMConvertToRadians(90);


// --------------------------------
//  �R���X�g���N�^
// --------------------------------
Sword_Attacking_2::Sword_Attacking_2(Sword* sword)
	: m_sword(sword)
	, m_position(5.0f, 0.0f, 5.0f)
	, m_velocity(0.0f, 0.0f, 0.0f)
	, m_angle(0.0f)
	, m_rot(0.0f, 0.0f, 0.0f)
	, m_totalSeconds(0.0f)
	, m_worldMatrix(DirectX::SimpleMath::Matrix::Identity)
{
}

// --------------------------------
//  �f�X�g���N�^
// --------------------------------
Sword_Attacking_2::~Sword_Attacking_2()
{
}


// --------------------------------
//  ����������
// --------------------------------
void Sword_Attacking_2::Initialize()
{
	m_particles = m_sword->GetPlayScene()->GetParticle();	// �p�[�e�B�N���̎擾
}


// --------------------------------
//  ��ԊJ�n����
// --------------------------------
void Sword_Attacking_2::PreUpdate()
{
	m_sword->SetAttackFlag(true); // �U���t���O�𗧂Ă�

	m_totalSeconds = 0.0f;	// �o�ߎ��Ԃ̏�����
	m_rootPos	.clear();	// ���{�̍��W�z����N���A
	m_tipPos	.clear();	// ��[�̍��W�z����N���A

	if (!m_sword->GetPlayScene()->GetEnemy()) return;
	m_sword->GetPlayScene()->GetEnemy()->CanHit(true); // �Փˉ\�ɂ���
}


// --------------------------------
//  �X�V����
// --------------------------------
void Sword_Attacking_2::Update(float elapsedTime)
{
	using namespace DirectX;

	// �o�ߎ��Ԃ��v��
	m_totalSeconds += elapsedTime;


	m_position = m_sword->GetPlayScene()->GetPlayer()->GetPosition();	// �v���C���[�̍��W���擾
	m_angle = m_sword->GetPlayScene()->GetPlayer()->GetAngle();		// �v���C���[�̉�]���擾

	// �A�j���[�V�����̍X�V
	UpdateAnimation();
	// ���[���h�s��̍X�V
	UpdateWorldMatrix();

	// �����蔻��̈ʒu��ݒ�
	m_sword->SetCollisionPosition(m_worldMatrix);

	// �G�t�F�N�g�`��p�̍��{�ƒ��_��`��
	GetCudgelBothEnds();
}


// --------------------------------
// �A�j���[�V�����X�V����
// --------------------------------
void Sword_Attacking_2::UpdateAnimation()
{
	// �U�����̉�]����
	if (m_totalSeconds <= ATTACK_TIME)
	{
		// �b���𐳋K������
		float t = m_totalSeconds / ATTACK_TIME;

		// �C�[�W���O�֐����g���ĉ�]���v�Z
		m_rot.y = MAX_SIDE_ANGLE					* Easing::easeOutBack(t);
		m_rot.x = INIT_ANGLE + MAX_VERTICAL_ANGLE	* Easing::easeOutBack(t);

		if (m_rot.y > MAX_SIDE_ANGLE)
		{
			// �G������ꍇ
			if (m_sword->GetPlayScene()->GetEnemy())
			{
				// �����蔻��𖳌��ɂ���
				m_sword->GetPlayScene()->GetEnemy()->CanHit(false);
			}
			m_sword->SetAttackFlag(false);
		}

		// ���W�A���ɕϊ�
		m_rot.x = DirectX::XMConvertToRadians(m_rot.x);
		m_rot.y = DirectX::XMConvertToRadians(m_rot.y);
	}
}


// --------------------------------
// ���[���h�s��̍X�V����
// --------------------------------
void Sword_Attacking_2::UpdateWorldMatrix()
{
	// ���[���h�s����X�V����
	m_worldMatrix = Matrix::CreateScale(Sword::SWORD_SCALE); // ���̃T�C�Y�̐ݒ�

	m_worldMatrix
		*= DirectX::SimpleMath::Matrix::CreateRotationX(RADIAN_90 / 2)		// ����90�x���Ɍ�����
		*= DirectX::SimpleMath::Matrix::CreateTranslation(MATRIX_DIRECTION)	// �����������炷
		*= DirectX::SimpleMath::Matrix::CreateRotationX(m_rot.x)			// �ガ�����̉�]�𔽉f
		*= DirectX::SimpleMath::Matrix::CreateRotationY(-m_angle)			// �v���C���[�̉��ɉ�]������
		*= DirectX::SimpleMath::Matrix::CreateRotationY(m_rot.y)			// �ガ�����̉�]�𔽉f
		*= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);		// �v���C���[�̈ʒu�ɐݒ�
}



// --------------------------------
//  ���[���W�̎擾����
// --------------------------------
void Sword_Attacking_2::GetCudgelBothEnds()
{
	// ���{�ƒ��_�̃��[���h���W�����ꂼ��擾
	m_rootPos.	push_back(Vector3::Transform(Vector3(0.0f, Sword::MODEL_ROOT_HEIGHT, 0.0f)	, m_worldMatrix));
	m_tipPos.	push_back(Vector3::Transform(Vector3(0.0f, Sword::MODEL_TOP_HEIGHT, 0.0f)	, m_worldMatrix));

	// �p�[�e�B�N���𐶐�
	CreateSwordParticle();
}


// --------------------------------
//  �a���G�t�F�N�g��������
// --------------------------------
void Sword_Attacking_2::CreateSwordParticle()
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
void Sword_Attacking_2::PostUpdate()
{
}


// --------------------------------
//  �I������
// --------------------------------
void Sword_Attacking_2::Finalize()
{
}


// --------------------------------
//  �Փˎ��C�x���g
// --------------------------------
void Sword_Attacking_2::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}
