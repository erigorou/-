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
	m_position{0, 0, 5},
	m_angle{0.f},
	m_worldMatrix{},
	m_currentState{}
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
	// �A�C�h�����O�X�e�[�g���擾
	m_playerIdling = std::make_unique<PlayerIdling>(this);
	// �A�C�h�����O�X�e�[�g�̏�����
	m_playerIdling->Initialize();
	// �_�b�W���O�X�e�[�g���擾
	m_playerDodging = std::make_unique<PlayerDodging>(this);
	// �_�b�W���O�X�e�[�g��������
	m_playerDodging->Initialize();
	// �ŏ��̃X�e�[�g��ݒ�
	m_currentState = m_playerIdling.get();
}


/// <summary>
/// �X�e�[�g��ύX����
/// </summary>
/// <param name="newState">�ύX�������X�e�[�g</param>
void Player::ChangeState(IState* newState)
{
	// ����X�V�������s��
	m_currentState->PostUpdate();
	// ���݂̃X�e�[�g��ύX����
	m_currentState = newState;
	// �V�����X�e�[�g�̎��O�X�V���s��
	m_currentState->PreUpdate();
}


// --------------------------------
//  �X�V����
// --------------------------------
void Player::Update(
	const DirectX::SimpleMath::Vector3 enemyPos,	// �G�̍��W
	const float elapsedTime				   // �OF����̌o�ߎ���
	)
{
	// �X�e�[�g�̍X�V
	m_currentState->Update(elapsedTime, m_position);

	// �����ꂱ����1�s�݂̂ɂ���@����������������������������������������������������������

	// �G�̈ʒu���r���ĉ�]�p���v�Z����
	CalculationAngle(enemyPos);
	//// �v���C���[�̈ړ�
	//MovePlayer();
	// ���[���h���W�̍X�V
	CalculationMatrix();
}


// ----------------------------------------
//  �G�̈ʒu����v���C���[�̉�]�p�����߂�
// ----------------------------------------
void Player::CalculationAngle(DirectX::SimpleMath::Vector3 const enemyPos)
{
	using namespace DirectX::SimpleMath;

	Vector3 forward = m_position - enemyPos;	// �G�̕������x�N�g���Ŏ擾
	forward.Normalize();						// ���K��

	Vector3 worldForward = Vector3::Forward;			// ���[���h���W�̑O���x�N�g�����쐬
	float dotProduct = forward.Dot(worldForward);		// ���ς��擾
	m_angle = acosf(dotProduct);						// ���ς���p�x���擾(�ʓx�@)

	Vector3 crossProduct = forward.Cross(worldForward);	// �J�����̑O�����x�N�g�����E�����Ɍ����Ă��邩�ǂ����ŕ���������
	m_angle = (crossProduct.y < 0)? -m_angle: m_angle;	// -180 ~ 180�Ɏ��߂�B
}



// --------------------------------
//  �ړ��̊Ǘ�
// --------------------------------
void Player::MovePlayer()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	// �L�[���͂��󂯕t����B
	Keyboard::State keyboardState = Keyboard::Get().GetState();

	m_velocity = SimpleMath::Vector3::Zero;	// ���x�l�����Z�b�g����

	if (keyboardState.Up)
		m_velocity += Vector3::Forward;	// �u���v�őO�i
	if (keyboardState.Down)
		m_velocity += Vector3::Backward;// �u���v�Ō��
	if (keyboardState.Left)
 		m_velocity += Vector3::Left;	// �u���v�ō��ړ�
	if (keyboardState.Right)
		m_velocity += Vector3::Right;	// �u���v�ŉE�ړ�

	if (m_velocity.LengthSquared() > 0.0f)		// �ړ��ʂ�����ꍇ�F
		m_velocity.Normalize();					// �ړ��ʂ𐳋K������

	m_velocity *= PLAYER_SPEED;					// �ړ��ʂ�␳����

	m_position += Vector3::Transform(-m_velocity, Matrix::CreateRotationY(-m_angle));	// �ړ��ʂ����W�ɔ��f

}


// --------------------------------
//  ���[���h�s��̌v�Z
// --------------------------------
void Player::CalculationMatrix()
{
	using namespace DirectX::SimpleMath;
	// �s��̌v�Z���s��
	m_worldMatrix = Matrix::Identity;		// �X�V���Ƃɏ��������s��
	m_worldMatrix
		*= Matrix::CreateTranslation(Vector3::Zero)					// ���_�Ɉړ�
		*= Matrix::CreateRotationY	(-m_angle)						// �G�̕���������悤�ɐݒ肷��
		*= Matrix::CreateTranslation(m_position);					// ���W���ړ�������
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

	// �X�e�[�g�ŕ`�悷��
	m_currentState->Render(
		context,
		states,
		view,
		projection);

	// �f�o�b�O�����uDebugString�v�ŕ\������
	auto debugString = resources->GetDebugString();
	debugString->AddString("m_angle : %f", XMConvertToDegrees( m_angle));
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