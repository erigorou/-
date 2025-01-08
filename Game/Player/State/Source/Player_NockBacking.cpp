#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/EasingFunctions.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Player/State/Header/Player_NockBacking.h"


// -------------------------------
// �R���X�g���N�^
// -------------------------------
PlayerNockBacking::PlayerNockBacking(Player* player)
	: m_player(player)
	, m_totalSeconds(0.0f)
	, m_elapsedTime(0.0f)
	, m_velocity(0.0f, 0.0f, -SPEED)
	, m_bodyTilt(0.0f)
{
}

// -------------------------------
// �f�X�g���N�^
// -------------------------------
PlayerNockBacking::~PlayerNockBacking()
{
}


// -------------------------------
// ����������
// -------------------------------
void PlayerNockBacking::Initialize()
{
}


// -------------------------------
// ���O�X�V����
// -------------------------------
void PlayerNockBacking::PreUpdate()
{
	// �o�ߎ��Ԃ̏�����
	m_totalSeconds = 0.f;
}


// -------------------------------
// �X�V����
// -------------------------------
void PlayerNockBacking::Update(const float& elapsedTime)
{
	// �o�ߎ��Ԃ��L�^
	m_elapsedTime = elapsedTime;

	m_position = m_player->GetPosition();

	DirectX::SimpleMath::Vector3 enemyPos = m_player->GetPlayScene()->GetEnemy()->GetPosition();

	m_player->SetAngle(Math::CalculationAngle(m_position, enemyPos));

	// �A�j���[�V�����̍X�V
	UpdateAnimation();

	// �X�e�[�g�J�n���玞�Ԃ��v���A��莞�Ԃŕʂ�State�֑J�ڂ�����
	m_player->TimeComparison(m_totalSeconds, END_TIME, m_player->GetPlayerIdlingState(), elapsedTime);
}


// -------------------------------
// �A�j���[�V�����X�V�p����
// -------------------------------
void PlayerNockBacking::UpdateAnimation()
{
	// �o�ߎ��Ԃ����Z
	m_totalSeconds += m_elapsedTime;

	// �m�b�N�o�b�N�̃A�j���[�V����
	if (m_totalSeconds <= NOCKBACKING_TIME)
	{
		NockBackAnimation();
	}
	// 
	else if (m_totalSeconds <= RETURN_TIME)
	{
		ReturnAnimation();
	}
}


// -------------------------------
// �m�b�N�o�b�N�A�j���[�V����
// -------------------------------
void PlayerNockBacking::NockBackAnimation()
{
	// ���Ԃ𐳋K������
	float t = std::min(1.0f, m_totalSeconds / NOCKBACKING_TIME);

	// �K�v���̎擾
	Vector3 playerPos = m_player->GetPlayScene()->GetEnemy()->GetPosition();
	Vector3 enemyPos = m_player->GetPosition();

	// �ǂ����̕����Ƀm�b�N�o�b�N����̂�
	m_nockBackAngle = Math::CalculationAngle(playerPos, enemyPos);
	Matrix rotMatrix = Matrix::CreateRotationY(-m_nockBackAngle);

	// �m�b�N�o�b�N��
	m_position += Vector3::Transform(m_velocity, rotMatrix) * m_elapsedTime;

	// sin�g�ŏ㉺�ړ�������
	m_position.y = Math::NormalizeSin(t) * MAX_UP_VALUE;

	// �X��
	m_bodyTilt.x = DirectX::XMConvertToRadians(-90 * Easing::easeOutBack(t));

	// �ݒ荀��
	m_player->SetPosition(m_position);
	m_player->SetAnimationRotate(m_bodyTilt);
}


// -------------------------------
// ���A�A�j���[�V����
// -------------------------------
void PlayerNockBacking::ReturnAnimation()
{
	// ���Ԃ𐳋K������
	float t = std::min(1.0f, (m_totalSeconds - NOCKBACKING_TIME) / (RETURN_TIME - NOCKBACKING_TIME));

	// �m�b�N�o�b�N��
	Matrix rotMatrix = Matrix::CreateRotationY(-m_nockBackAngle);
	m_position += Vector3::Transform(m_velocity, rotMatrix) * m_elapsedTime;

	// sin�g�ŏ㉺�ړ�������
	m_position.y = Math::NormalizeSin(t) * RETURN_UP_VALUE;

	// �X��
	m_bodyTilt.x = DirectX::XMConvertToRadians(-90 - 270 * Easing::easeOutBack(t));

	m_player->SetPosition(m_position);
	m_player->SetAnimationRotate(m_bodyTilt);
}



// -------------------------------
// �L�[���̓C�x���g
// -------------------------------
void PlayerNockBacking::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}
void PlayerNockBacking::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}

// -------------------------------
// ����X�V����
// -------------------------------
void PlayerNockBacking::PostUpdate()
{
}


// -------------------------------
// �I������
// -------------------------------
void PlayerNockBacking::Finalize()
{
}
