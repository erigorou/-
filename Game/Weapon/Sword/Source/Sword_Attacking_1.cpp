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

#include "Game/Weapon/Sword/Sword.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Sword/Header/Sword_Attacking_1.h"


// �Œ�l
const float Sword_Attacking_1::RADIAN_90 = DirectX::XMConvertToRadians(90);
const float Sword_Attacking_1::ATTACK_TIME  = 0.3f;


// �R���X�g���N�^
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
}

// �f�X�g���N�^
Sword_Attacking_1::~Sword_Attacking_1()
{
}


// ����������
void Sword_Attacking_1::Initialize()
{
	using namespace DirectX::SimpleMath;
	// ���f�����擾
	m_model = m_sword->GetModel();
	// ���[���h�s���������
	m_worldMatrix = Matrix::Identity;

	// ���E�{�b�N�X�𐶐�
	// ������OBB�ɂ���
	m_boundingBox = DirectX::BoundingOrientedBox();

	// OBB�̐��������錴�^
	m_originalBox = DirectX::BoundingOrientedBox();
}


// ���O����
void Sword_Attacking_1::PreUpdate()
{
	// �o�ߎ��Ԃ̃��Z�b�g
	m_totalSeconds = 0.0f;
	// ���E�{�b�N�X�̏ꏊ�����Z�b�g
	m_boundingBox.Center = DirectX::SimpleMath::Vector3::Zero;
}

// �X�V����
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

	// ����U�鎞�ԓ��Ȃ�
	if (m_totalSeconds < ATTACK_TIME)
	{
		// y���ɍX�V
		m_rot.y = XMConvertToRadians(m_totalSeconds * 1000);
	}

	// ���[���h�s����X�V����
	m_worldMatrix = Matrix::CreateScale(Sword::SWORD_SCALE);							// �T�C�Y�̐ݒ�

	m_worldMatrix
		*= SimpleMath::Matrix::CreateRotationX(RADIAN_90)								// 90�x���Ɍ�����
		*= SimpleMath::Matrix::CreateTranslation(Vector3(1.0f, 2.0f, 0.0f))				// ���_�ŁA�����������炷
		*= SimpleMath::Matrix::CreateRotationY(-m_angle)								// �v���C���[�̉��ɂȂ�悤��]���s��
		*= SimpleMath::Matrix::CreateRotationY(m_rot.y)									// ��]
		*= SimpleMath::Matrix::CreateTranslation(m_position);							// �v���C���̈ʒu�ɐݒ肷��


	// �����蔻��ɂ����[���h�s��̌v�Z���s��
	m_originalBox.Transform(m_boundingBox, m_worldMatrix);


	// 1�b�o�߂ŃX�e�[�g�ύX
	if (m_totalSeconds >= 1.0f)
	{
		m_sword->ChangeState(m_sword->GetIdlingState());
	}
}


// ���㏈��
void Sword_Attacking_1::PostUpdate()
{
}


// �`�揈��
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
	debugString->AddString("sword, %f : %f : %f", m_position.x, m_position.y, m_position.z);
#endif // _DEBUG
}


// �I������
void Sword_Attacking_1::Finalize()
{
}