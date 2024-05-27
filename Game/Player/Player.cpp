// ----------------
//
// �����Y�̋���
//
// ----------------

#include "pch.h"
#include <Model.h>
#include <cassert>

#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"

#include "Game/Player/Player.h"


// �����ŐÓI�����o�[�ϐ����`����
const DirectX::SimpleMath::Vector3 Player::HOME_POSITION(0.0f);
const float Player::PLAYER_SPEED = 0.05f;


// --------------------------------
//  �R���X�g���N�^
// --------------------------------
Player::Player()
	:
	m_model{},
	m_position{},
	m_angle{},
	m_cameraAngle{},
	m_worldMatrix{}
{
}

// --------------------------------
//  �f�X�g���N�^
// --------------------------------
Player::~Player()
{
}

// --------------------------------
//  �C�j�V�����C�Y
// --------------------------------
void Player::Initialize(
	 ID3D11Device* device,
	const ID3D11DeviceContext* context,
	const DirectX::CommonStates* states
	)
{
	using namespace DirectX;

	// ���f����ǂݍ��ޏ���
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// ���f����ǂݍ���
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/momotaro.cmo", *fx);

	// ���W�̏����ݒ�
	m_position = SimpleMath::Vector3(4.0f, 0.0f, 2.0f);
	// ���x�̏�����
	m_velocity = SimpleMath::Vector3::Zero;
	// ��]�̏����l
	m_angle = 0.0f;
	// �J�����̉�]�p�̏�����
	m_cameraAngle = 0.0f;
}

// --------------------------------
//  �X�V����
// --------------------------------
void Player::Update(
	DirectX::SimpleMath::Vector3 enemyPos,
	float cameraAngle
	)
{
	using namespace DirectX::SimpleMath;

	// ���̃N���X�ŃJ�����̃A���O�����g�p�ł���悤�ɂ���
	m_cameraAngle = cameraAngle;

	m_angle = -cameraAngle;

	// �v���C���[�̉�]�����������v�Z���s��
	Matrix matrix = Matrix::CreateRotationY(-cameraAngle);	// �J�����̉�]��^����

	// �v���C���[�̈ړ�
	MovePlayer();

	// ��]���������Ď��ۂɈړ�����
	m_position += Vector3::Transform(m_velocity, matrix);

	// ���[���h�}�g���N�X�̌v�Z
	CalculationMatrix();
}



// --------------------------------
//  �ړ��̊Ǘ�
// --------------------------------
void Player::MovePlayer()
{
	using namespace DirectX;

	// �L�[���͂��󂯕t����B
	Keyboard::State keyboardState = Keyboard::Get().GetState();

	// ���x�l�����Z�b�g����
	m_velocity = SimpleMath::Vector3::Zero;

	if (keyboardState.Up)	// �u���v�őO�i
		m_velocity += SimpleMath::Vector3::Forward;
	if (keyboardState.Down)	// �u���v�Ō��
		m_velocity += SimpleMath::Vector3::Backward;
	if (keyboardState.Left)	// �u���v�ō��ړ�
 		m_velocity += SimpleMath::Vector3::Left;
	if (keyboardState.Right)// �u���v�ŉE�ړ�
		m_velocity += SimpleMath::Vector3::Right;

	if (keyboardState.A)
		m_angle += 1.0f;
	if (keyboardState.D)
		m_angle -= 1.0f;

	(m_angle <= 360) ? m_angle : 0.0f;


	// �ړ��ʂ𐳋K������
	m_velocity.Normalize();

	// �ړ��ʂ�␳����
	m_velocity *= PLAYER_SPEED;
}


// --------------------------------
//  ���[���h�s��̌v�Z
// --------------------------------
void Player::CalculationMatrix()
{
	using namespace DirectX;

	// �s��̌v�Z���s��
	m_worldMatrix = SimpleMath::Matrix::Identity;
	m_worldMatrix
		*= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3::Zero)		// �����n�_�Ɉړ�
		*= SimpleMath::Matrix::CreateRotationY(m_angle)							// ��]��K�p
		*= SimpleMath::Matrix::CreateTranslation(m_position);					// ���W���ړ�������
}


// --------------------------------
//  �\������
// --------------------------------
void Player::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection,
	const CommonResources* resources
	)
{
	using namespace DirectX;

	// ���f����`�悷��
	m_model->Draw(context, *states, m_worldMatrix, view, projection);

	// �f�o�b�O�����uDebugString�v�ŕ\������
	auto debugString = resources->GetDebugString();
	debugString->AddString("m_angle : %f", m_angle);
	debugString->AddString("m_position.x : %f", m_position.x);
	debugString->AddString("m_position.y : %f", m_position.y);
	debugString->AddString("m_position.z : %f", m_position.z);
}


// --------------------------------
//  �I������
// --------------------------------
void Player::Finalize()
{
}