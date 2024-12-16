#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/EasingFunctions.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Player/State/Header/Player_Dodging.h"


// --------------------------------------
// �R���X�g���N�^
// --------------------------------------
PlayerDodging::PlayerDodging(Player* player)
	:
	 m_player(player)
	,m_model(nullptr)
	,m_totalSeconds(0.0f)
	,m_finishTime(0.0f)
	,m_rollingValue()

{
}

// --------------------------------------
// �f�X�g���N�^
// --------------------------------------
PlayerDodging::~PlayerDodging()
{
}


// -------------------------------------
// ����������
// --------------------------------------
void PlayerDodging::Initialize(DirectX::Model* model)
{
	// ���f�����擾����
	m_model = model;
}


// -------------------------------------
// �v���C���[�̎��O�X�V����
// --------------------------------------
void PlayerDodging::PreUpdate()
{
	// �o�ߎ��Ԃ̏�����
	m_totalSeconds = 0.0f;
	// �ړ������̐ݒ�
	m_velocity = DirectX::SimpleMath::Vector3::Forward * DODGING_SPEED;
}


// -------------------------------------
// �v���C���[�̍X�V����
// --------------------------------------
void PlayerDodging::Update(const float& elapsedTime)
{
	float t = elapsedTime;	
	// �o�ߎ��Ԃ����Z
	m_totalSeconds += t;
	// �A�j���[�V�����̍X�V
	UpdateAnimation(m_totalSeconds);
	// �X�e�[�g�J�n���玞�Ԃ��v���A��莞�Ԃŕʂ�State�֑J�ڂ�����
	m_player->TimeComparison(m_totalSeconds, ANIMATION_END_TIME, m_player->GetPlayerIdlingState(), elapsedTime);
}


// -------------------------------------
// �v���C���[�̃A�j���[�V�����p�X�V����
// --------------------------------------
void PlayerDodging::UpdateAnimation(float totalTime)
{
	// �v���C���[�̍��W���擾
	m_position = m_player->GetPosition();

	// ���[�����O�A�j���[�V����
	RollingAnimation(totalTime);
	// �L�����N�^�[�̈ړ��𒲐�
	AdjustCharacterTransition(totalTime);
}


// -------------------------------
// ���[�����O�A�j���[�V����
// -------------------------------
void PlayerDodging::RollingAnimation(float totalTime)
{
	// �C�[�W���O�̗v�f��
	float value;

	// ��]�����̕���
	if (totalTime <= SPIN_TIME)
	{
		value = Easing::easeInOutQuad(totalTime / SPIN_TIME);

		m_rollingValue.x = DirectX::XMConvertToRadians(value * ROLLING_ROT + ROLLING_REST_ROT);
	}
	// ���[�����O��̍d������
	else if (totalTime <= SPIN_REST_TIME)
	{
		// SPIN_TIME ���z�������̎��ԂŊ������v�Z
		value = (totalTime - SPIN_TIME) / (SPIN_REST_TIME - SPIN_TIME);
		float easeValue = Easing::easeOutBack(value);

		float rot = ROLLING_REST_ROT - (easeValue * ROLLING_REST_ROT);

		// ��]�����W�A���ɕϊ�
		m_rollingValue.x = DirectX::XMConvertToRadians(rot);
	}
	// ��]��K�p
	m_player->SetAnimarionRotate(m_rollingValue);
}


// -----------------------------------------
// �v���C���[�̈ړ��𒲐�
// -----------------------------------------
void PlayerDodging::AdjustCharacterTransition(float totalTime)
{
	// �C�[�W���O�̗v�f��
	float value;

	// ���[�����O�����̕���
	if (totalTime <= ANIMATION_TIME)
	{
		// �C�[�W���O���|���邽�߂̎���
		value = Easing::easeOutCubic(totalTime / ANIMATION_TIME);
		m_position.y = value * UP_VALUE;
	}
	// ���[�����O��̍d������
	else if (totalTime - ANIMATION_TIME <= DODGING_RECOVERY_TIME - ANIMATION_TIME)
	{
		// �㏈�����s��
		value = Easing::easeInOutQuad((totalTime - ANIMATION_TIME) / (DODGING_RECOVERY_TIME - ANIMATION_TIME));
		m_position.y = UP_VALUE - (value * UP_VALUE);
	}

	// �v���C���[�̈ړ���K�p
	ApplyPlayerMovement(m_position);
}



// �v���C���[�̈ړ���K�p
void PlayerDodging::ApplyPlayerMovement(DirectX::SimpleMath::Vector3& parentPos)
{
	// �v���C���[�̉�]���擾
	Matrix angle = Matrix::CreateRotationY(-m_player->GetAngle());
	// ���x��ݒ�
	m_velocity *= DODGE_FUNCTION;
	// �[����������΂��B
	m_velocity = Math::truncate_vector(m_velocity, TRUNCATION_DIGIT);
	// �ړ��ʂ����W�ɔ��f�����Ȃ�����W���ړ�������B
	parentPos += Vector3::Transform(m_velocity, angle);
	// �v���C���[�̍��W���X�V
	m_player->SetPosition(parentPos);
}




// �L�[����
void PlayerDodging::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}

void PlayerDodging::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}


// ����X�V����
void PlayerDodging::PostUpdate()
{
	m_position.y = 0.0f;
	// �v���C���[�̍��W���X�V
	m_player->SetPosition(m_position);
}


// �`�揈��
void PlayerDodging::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
	)
{
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(states);
	UNREFERENCED_PARAMETER(view);
	UNREFERENCED_PARAMETER(projection);
	UNREFERENCED_PARAMETER(m_model);

#ifdef _DEBUG
	// �R�������\�[�X���擾����
	CommonResources* resources = CommonResources::GetInstance();
	// �f�o�b�O�����̕`��
	auto debugString = resources->GetDebugString();
	debugString->AddString("");
#endif // _DEBUG
}


// �I������
void PlayerDodging::Finalize()
{
}
