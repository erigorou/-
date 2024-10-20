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
#include "Game/Weapon/Sword/Header/Sword_Attacking_3.h"


// �Œ�l
const float Sword_Attacking_3::RADIAN_90 = DirectX::XMConvertToRadians(90);
const float Sword_Attacking_3::ATTACK_TIME  = 0.7f;


// �R���X�g���N�^
Sword_Attacking_3::Sword_Attacking_3(Sword* sword)
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
}

// �f�X�g���N�^
Sword_Attacking_3::~Sword_Attacking_3()
{
}


// ����������
void Sword_Attacking_3::Initialize()
{
	using namespace DirectX::SimpleMath;
	// ���f�����擾
	m_model = m_sword->GetModel();
	// ���[���h�s���������
	m_worldMatrix = Matrix::Identity;
}


// ���O����
void Sword_Attacking_3::PreUpdate()
{
	// �o�ߎ��Ԃ̃��Z�b�g
	m_totalSeconds = 0.0f;

	// �������悤�ɂ���
	m_sword->GetPlayScene()->GetEnemy()->CanHit(true);
}


// �X�V����
void Sword_Attacking_3::Update(float elapsedTime)
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
		t = m_totalSeconds / ATTACK_TIME;  // �i�s�x��0����1��

		// ������ɐ؂�グ��悤�ɁAX����]�𒲐�
		m_rot.x = 80.0f - 100.0f * Easying::easeOutBack(t);  // �؂�グ�p�x��傫�߂ɐݒ�

		m_rot.x = XMConvertToRadians(m_rot.x);  // ���W�A���ɕϊ�
	}
	else
	{
		// �U�����Ԃ��߂����瓖���蔻��𖳌��ɂ���
		m_sword->GetPlayScene()->GetEnemy()->CanHit(false);
	}

	// ���[���h�s����X�V����
	m_worldMatrix = Matrix::CreateScale(Sword::SWORD_SCALE);							// ���̃T�C�Y�̐ݒ�


	// 1. �����̂̃A�j���[�V�����i��]��ړ��j
	m_worldMatrix
		*= SimpleMath::Matrix::CreateRotationZ(m_rot.x) // X����]��K�p
		*= SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(-90.0f))
		*= SimpleMath::Matrix::CreateTranslation(Vector3(2.0f, 2.0f, -2.0f));

	// 2. �v���C���[�̈ʒu�Ɖ�]��K�p
	m_worldMatrix
		*= SimpleMath::Matrix::CreateRotationY(-m_angle) // �v���C���[�̕����ɉ�]
		*= SimpleMath::Matrix::CreateTranslation(m_position); // �v���C���[�̈ʒu�ɐݒ�
	// �����蔻��̈ʒu��ݒ�
	m_sword->SetCollisionPosition(m_worldMatrix);

	// �U�����I�������X�e�[�g��IdlingState�ɖ߂�
	if (m_totalSeconds >= 1.0f)	m_sword->ChangeState(m_sword->GetIdlingState());
}




// ���㏈��
void Sword_Attacking_3::PostUpdate()
{
}


// �`�揈��
void Sword_Attacking_3::Render(ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	CommonResources* resources = CommonResources::GetInstance();

	// ���f����`�悷��
	m_model->Draw(context, *states, m_worldMatrix, view, projection);


#ifdef _DEBUG
	auto debugString = resources->GetDebugString();
	debugString->AddString("rot.x : %f", DirectX::XMConvertToDegrees(m_rot.x));
#endif // _DEBUG
}


// �I������
void Sword_Attacking_3::Finalize()
{
}


void Sword_Attacking_3::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}
