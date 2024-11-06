/// ---------------------------------------
///
/// �v���C���[���U�������鏈��
///
/// ---------------------------------------

#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Sound/Sound.h"

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Player/State/Header/Player_Attacking_4.h"
 

// --------------------------------
//  �R���X�g���N�^
// --------------------------------
PlayerAttacking_4::PlayerAttacking_4(Player* player)
	:
	 m_player(player)
	,m_totalSeconds()
	,m_attackElapsedTime(0.0f)
	,m_model()
{
}

// --------------------------------
//  �f�X�g���N�^
// --------------------------------
PlayerAttacking_4::~PlayerAttacking_4()
{
}

// --------------------------------
//  ����������
// --------------------------------
void PlayerAttacking_4::Initialize(DirectX::Model* model)
{
	// ���f�����擾����
	m_model = model;
}

// --------------------------------
//  ��ԊJ�n����
// --------------------------------
void PlayerAttacking_4::PreUpdate()
{
	// �o�ߎ��Ԃ̏�����
	m_totalSeconds = 0.f;


	// ���̍U����ԂɕύX
	m_player->GetPlayScene()->GetSword()->ChangeState(
		m_player->GetPlayScene()->GetSword()->GetAttacking_4State()
	);

	Sound::PlaySE(Sound::SE_TYPE::PLAYER_ATTACK2);
}

// --------------------------------
//  �X�V����
// --------------------------------
void PlayerAttacking_4::Update(const float& elapsedTime)
{
	// ���Ԃ��v�����A��莞�Ԍo�߂ŃX�e�[�g��J��
	m_player->TimeComparison(m_totalSeconds, Player::APPLIED_ATTACK_TIME, m_player->GetPlayerIdlingState(), elapsedTime);
}


// --------------------------------
//  �L�[���͏����i�u�ԁj
// --------------------------------
void PlayerAttacking_4::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}

// --------------------------------
//  �L�[���͏����i�p���j
// --------------------------------
void PlayerAttacking_4::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}


// --------------------------------
//  ��ԏI������
// --------------------------------
void PlayerAttacking_4::PostUpdate()
{
	// ������U����ԂɕύX
	m_player->GetPlayScene()->GetSword()->ChangeState(
		m_player->GetPlayScene()->GetSword()->GetIdlingState()
	);
}


// --------------------------------
//  �`�揈��
// --------------------------------
void PlayerAttacking_4::Render(
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

	// �R�������\�[�X���擾����
	CommonResources* resources = CommonResources::GetInstance();


	// �f�o�b�O�����uDebugString�v�ŕ\������
	auto debugString = resources->GetDebugString();
	debugString->AddString("");
}


// --------------------------------
//  �I������
// --------------------------------
void PlayerAttacking_4::Finalize()
{
}