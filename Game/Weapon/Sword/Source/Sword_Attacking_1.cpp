/// ---------------------------
///
/// �v���C���[�̌��̍U���i�~�؂�j(���])
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
#include "Game/Messenger/EventMessenger.h"

#include "Game/Player/Player.h"
#include "Game/Boss/Boss.h"
#include "Game/Weapon/Sword/Header/Sword_Attacking_1.h"


// --------------------------------
//  �Œ�l
// --------------------------------
const float Sword_Attacking_1::RADIAN_90 = DirectX::XMConvertToRadians(90);


// --------------------------------
//  �R���X�g���N�^
// --------------------------------
Sword_Attacking_1::Sword_Attacking_1(Sword* sword)
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
Sword_Attacking_1::~Sword_Attacking_1()
{
}


// --------------------------------
//  ����������
// --------------------------------
void Sword_Attacking_1::Initialize()
{
	m_particles = m_sword->GetPlayScene()->GetParticle();	// �p�[�e�B�N���̎擾
}


// --------------------------------
//  ��ԊJ�n����
// --------------------------------
void Sword_Attacking_1::PreUpdate()
{
	// �U���t���O�𗧂Ă�
	m_sword->SetAttackFlag(true);

	m_totalSeconds = 0.0f;	// ���Ԍo�߂̃��Z�b�g

	m_rootPos.clear();	// ���{�̍��W���N���A
	m_tipPos.clear();	// ��[�̍��W���N���A

	bool canHit = true;

	// �Փˉ\�t���O��G�S�̂ɕt�^
	EventMessenger::Execute("EnemyCanHit",&canHit);
}


// --------------------------------
//  �X�V����
// --------------------------------
void Sword_Attacking_1::Update(float elapsedTime)
{
	// �o�ߎ��Ԃ��v��
	m_totalSeconds += elapsedTime;

	m_position	= m_sword->GetPlayScene()->GetPlayer()->GetPosition();	// �v���C���[�̍��W���擾
	m_angle		= m_sword->GetPlayScene()->GetPlayer()->GetAngle();		// �v���C���[�̉�]���擾

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
//  �A�j���[�V�����X�V����
// --------------------------------
void Sword_Attacking_1::UpdateAnimation()
{
	// �C�[�W���O�֐����g���ĉ�]���v�Z
	float t = 0.0f;

	if (m_totalSeconds <= ATTACK_TIME)
	{
		// ���Ԃ̐��K��
		t = m_totalSeconds / ATTACK_TIME;

		m_rot.y = MAX_SIDE_ANGLE					* Easing::easeOutBack(t);
		m_rot.x = INIT_ANGLE - MAX_VERTICAL_ANGLE	* Easing::easeOutBack(t);

		if (m_rot.y < MAX_SIDE_ANGLE)
		{
			// �{�X�����݂���ꍇ
			if (m_sword->GetPlayScene()->GetBoss())
			{
				//// �����蔻��𖳌��ɂ���
				//m_sword->GetPlayScene()->GetEnemy()->CanHit(false);
			}
			m_sword->SetAttackFlag(false);
		}

		// �ݒ�
		m_rot.x = DirectX::XMConvertToRadians(m_rot.x);
		m_rot.y = DirectX::XMConvertToRadians(m_rot.y);
	}
}


// --------------------------------
// ���[���h�s��̍X�V����
// --------------------------------
void Sword_Attacking_1::UpdateWorldMatrix()
{	// ���[���h�s����X�V����
	m_worldMatrix = Matrix::CreateScale(Sword::SWORD_SCALE);	// ���̃T�C�Y�̐ݒ�

	m_worldMatrix
		*= DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(180))	// ���]
		*= DirectX::SimpleMath::Matrix::CreateRotationX(RADIAN_90)							// ����90�x���Ɍ�����
		*= DirectX::SimpleMath::Matrix::CreateTranslation(Vector3(1.0f, 2.0f, 0.0f))		// �����������炷
		*= DirectX::SimpleMath::Matrix::CreateRotationX(m_rot.x)							// �ガ�����̉�]�𔽉f
		*= DirectX::SimpleMath::Matrix::CreateRotationY(-m_angle)							// �v���C���[�̉��ɉ�]������
		*= DirectX::SimpleMath::Matrix::CreateRotationY(m_rot.y)							// �ガ�����̉�]�𔽉f
		*= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);						// �v���C���[�̈ʒu�ɐݒ�

	m_sword->SetWorldMatrix(m_worldMatrix);	// ���[���h�s���ݒ�
}



// --------------------------------
//  �ʒ��_�̎擾
// --------------------------------
void Sword_Attacking_1::GetCudgelBothEnds()
{
	// ���{�ƒ��_�̃��[���h���W�����ꂼ��擾
	m_rootPos.push_back(Vector3::Transform(Vector3(0.0f, Sword::MODEL_ROOT_HEIGHT, 0.0f), m_worldMatrix));
	m_tipPos.push_back(Vector3::Transform(Vector3(0.0f, Sword::MODEL_TOP_HEIGHT, 0.0f), m_worldMatrix));

	// �p�[�e�B�N���𐶐�
	CreateSwordParticle();
}


// --------------------------------
//  �a���G�t�F�N�g�̐�������
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
		EventMessenger::Execute("CreateSwordTrail", &ver);	// �p�[�e�B�N���̐���
	}
}


// --------------------------------
//  ��ԏI������
// --------------------------------
void Sword_Attacking_1::PostUpdate()
{
	bool canHit = false;

	// �Փˉ\�t���O��G�S�̂ɕt�^
	EventMessenger::Execute("EnemyCanHit", &canHit);
}



// --------------------------------
//  �I������
// --------------------------------
void Sword_Attacking_1::Finalize()
{
}


// --------------------------------
//  �Փˏ����C�x���g
// --------------------------------
void Sword_Attacking_1::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}
