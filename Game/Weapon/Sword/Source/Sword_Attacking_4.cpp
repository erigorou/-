/// ---------------------------
///
/// �v���C���[�̌��̍U���i���������j(���])
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

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Weapon/Sword/Header/Sword_Attacking_4.h"


// --------------------------------
//  �Œ�l
// --------------------------------
const float Sword_Attacking_4::RADIAN_90 = DirectX::XMConvertToRadians(90);
const float Sword_Attacking_4::ATTACK_TIME  = 0.7f;


// --------------------------------
//  �R���X�g���N�^
// --------------------------------
Sword_Attacking_4::Sword_Attacking_4(Sword* sword)
	: m_sword		(sword)
	, m_position	(5.0f, 0.0f, 5.0f)
	, m_velocity	(0.0f, 0.0f, 0.0f)
	, m_angle		(0.0f)
	, m_rot			(0.0f, 0.0f, 0.0f)
	, m_totalSeconds(0.0f)
	, m_worldMatrix	(DirectX::SimpleMath::Matrix::Identity)
{
}


// --------------------------------
//  �f�X�g���N�^
// --------------------------------
Sword_Attacking_4::~Sword_Attacking_4()
{
}


// --------------------------------
//  ����������
// --------------------------------
void Sword_Attacking_4::Initialize()
{
	m_particles = m_sword->GetPlayScene()->GetParticle();	// �p�[�e�B�N���̎擾
}


// --------------------------------
//  ��ԊJ�n����
// --------------------------------
void Sword_Attacking_4::PreUpdate()
{
	// �U���̃t���O�𗧂Ă�
	m_sword->SetAttackFlag(true);

	// �o�ߎ��Ԃ̃��Z�b�g
	m_totalSeconds = 0.0f;

	// �������悤�ɂ���
	m_sword->GetPlayScene()->GetEnemy()->CanHit(true);

	m_rootPos.clear();
	m_tipPos.clear();
}


// --------------------------------
//  �X�V����
// --------------------------------
void Sword_Attacking_4::Update(float elapsedTime)
{
	using namespace DirectX;

	// �o�ߎ��Ԃ��v��
	m_totalSeconds += elapsedTime;

	// �v���C���[�̍��W���擾
	m_position = m_sword->GetPlayScene()->GetPlayer()->GetPosition();
	// �v���C���[�̉�]���擾
	m_angle = m_sword->GetPlayScene()->GetPlayer()->GetAngle();

	float t = 0.0f;

	if (m_totalSeconds <= ATTACK_TIME)
	{
		t = m_totalSeconds / ATTACK_TIME;  // �i�s�x��0����1��

		// ������ɐ؂�グ��悤�ɁAX����]�𒲐�
		m_rot.x = -20.0f + 150.0f * Easing::easeOutElastic(t);  // �؂�グ�p�x��傫�߂ɐݒ�

		m_rot.x = XMConvertToRadians(m_rot.x);  // ���W�A���ɕϊ�
	}
	else
	{
		// �U�����Ԃ��߂����瓖���蔻��𖳌��ɂ���
		m_sword->GetPlayScene()->GetEnemy()->CanHit(false);
	}

	// ���[���h�s����X�V����
	m_worldMatrix = Matrix::CreateScale(Sword::SWORD_SCALE);		// ���̃T�C�Y�̐ݒ�


	// 1. �����̂̃A�j���[�V�����i��]��ړ��j
	m_worldMatrix
		*= SimpleMath::Matrix::CreateRotationZ(m_rot.x)							// Z����]��K�p
		*= SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(-90.0f))		// Y����]��K�p
		*= SimpleMath::Matrix::CreateTranslation(Vector3(2.0f, 2.0f, -2.0f));	// ���_����ړ�

	// 2. �v���C���[�̈ʒu�Ɖ�]��K�p
	m_worldMatrix
		*= SimpleMath::Matrix::CreateRotationY(-m_angle)		// �v���C���[�̉�]��t�^
		*= SimpleMath::Matrix::CreateTranslation(m_position);	// �v���C���[�̈ʒu��t�^

	// �Փ˔���̈ʒu��ݒ�
	m_sword->SetCollisionPosition(m_worldMatrix);

	// ���[�̍��W�擾
	GetCudgelBothEnds();
}


// --------------------------------
//  ���[�̍��W�擾����
// --------------------------------
void Sword_Attacking_4::GetCudgelBothEnds()
{
	// ���{�ƒ��_�̃��[���h���W�����ꂼ��擾
	m_rootPos	.push_back(Vector3::Transform(Vector3(0.0f, Sword::MODEL_ROOT_HEIGHT, 0.0f), m_worldMatrix));
	m_tipPos	.push_back(Vector3::Transform(Vector3(0.0f,	Sword::MODEL_TOP_HEIGHT	, 0.0f), m_worldMatrix));

	// �p�[�e�B�N���𐶐�
	CreateSwordParticle();
}



// --------------------------------
//  �a���G�t�F�N�g�̐�������
// --------------------------------
void Sword_Attacking_4::CreateSwordParticle()
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
void Sword_Attacking_4::PostUpdate()
{
}


// --------------------------------
//  �I������
// --------------------------------
void Sword_Attacking_4::Finalize()
{
}


// --------------------------------
//  �Փˏ����C�x���g
// --------------------------------
void Sword_Attacking_4::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}
