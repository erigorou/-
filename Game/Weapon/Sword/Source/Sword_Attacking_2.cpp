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

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Weapon/Sword/Header/Sword_Attacking_2.h"


// --------------------------------
//  �Œ�l
// --------------------------------
const float Sword_Attacking_2::RADIAN_90 = DirectX::XMConvertToRadians(90);
const float Sword_Attacking_2::ATTACK_TIME  = 0.5f;


// --------------------------------
//  �R���X�g���N�^
// --------------------------------
Sword_Attacking_2::Sword_Attacking_2(Sword* sword)
	: m_sword		(sword)
	, m_position	(5.0f, 0.0f, 5.0f)
	, m_velocity	(0.0f, 0.0f, 0.0f)
	, m_angle		(0.0f)
	, m_rot			(0.0f, 0.0f, 0.0f)
	, m_totalSeconds(0.0f)
	, m_worldMatrix	(DirectX::SimpleMath::Matrix::Identity)
	, m_model		(nullptr)
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
	m_model		= m_sword->GetModel();						// ���f���̎擾
	m_particles = m_sword->GetPlayScene()->GetParticle();	// �p�[�e�B�N���̎擾
}


// --------------------------------
//  ��ԊJ�n����
// --------------------------------
void Sword_Attacking_2::PreUpdate()
{
	m_totalSeconds = 0.0f;								// ���Ԍo�߂̃��Z�b�g
	m_sword->GetPlayScene()->GetEnemy()->CanHit(true);	// �Փˉ\�t���O��L����

	m_rootPos	.clear();	// ���{�̍��W���N���A
	m_tipPos	.clear();	// ��[�̍��W���N���A
}


// --------------------------------
//  �X�V����
// --------------------------------
void Sword_Attacking_2::Update(float elapsedTime)
{
	using namespace DirectX;

	// �o�ߎ��Ԃ��v��
	m_totalSeconds += elapsedTime;

	// �v���C���[�̍��W���擾
	m_position = m_sword->GetPlayScene()->GetPlayer()->GetPosition();
	// �v���C���[�̉�]���擾
	m_angle = m_sword->GetPlayScene()->GetPlayer()->GetAngle();

	// �C�[�W���O�֐����g���ĉ�]���v�Z
	float t = 0.0f;

	if (m_totalSeconds <= ATTACK_TIME)
	{
		t = m_totalSeconds / ATTACK_TIME;
		m_rot.y = -250.0f * Easing::easeOutBack(t);
		m_rot.x = 10 - 40 * Easing::easeOutBack(t);

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
		*= SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(180))					// ���]
		*= SimpleMath::Matrix::CreateRotationX(RADIAN_90)								// ����90�x���Ɍ�����
		*= SimpleMath::Matrix::CreateTranslation(Vector3(1.0f, 2.0f, 0.0f))				// �����������炷
		*= SimpleMath::Matrix::CreateRotationX(m_rot.x)									// �ガ�����̉�]�𔽉f
		*= SimpleMath::Matrix::CreateRotationY(-m_angle)								// �v���C���[�̉��ɉ�]������
		*= SimpleMath::Matrix::CreateRotationY(m_rot.y)									// �ガ�����̉�]�𔽉f
		*= SimpleMath::Matrix::CreateTranslation(m_position);							// �v���C���[�̈ʒu�ɐݒ�

	// �����蔻��̈ʒu��ݒ�
	m_sword->SetCollisionPosition(m_worldMatrix);

	// �G�t�F�N�g�`��p�̍��{�ƒ��_��`��
	GetCudgelBothEnds();
}


// --------------------------------
//  �ʒ��_�̎擾
// --------------------------------
void Sword_Attacking_2::GetCudgelBothEnds()
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
//  �`�揈��
// --------------------------------
void Sword_Attacking_2::Render(ID3D11DeviceContext* context,
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
void Sword_Attacking_2::Finalize()
{
}


// --------------------------------
//  �Փˏ����C�x���g
// --------------------------------
void Sword_Attacking_2::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}
