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
#include "Game/Weapon/Sword/Sword.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Player/State/Header/Player_Attacking_1.h"



// �R���X�g���N�^
PlayerAttacking_1::PlayerAttacking_1(Player* player)
	:
	 m_player(player)
	,m_totalSeconds()
	,m_attackElapsedTime()
	,m_model()
{
}

// �f�X�g���N�^
PlayerAttacking_1::~PlayerAttacking_1()
{
}


// ����������
void PlayerAttacking_1::Initialize(DirectX::Model* model)
{
	// ���f�����擾����
	m_model = model;

}

// ���O�X�V����
void PlayerAttacking_1::PreUpdate()
{
	// �o�ߎ��Ԃ̏�����
	m_totalSeconds = 0.f;
	
	// ������U����ԂɕύX
	m_player->GetPlayScene()->GetSword()->ChangeState(
		m_player->GetPlayScene()->GetSword()->GetAttacking_1State()
	);

	Sound::PlaySE(Sound::SE_TYPE::PLAYER_ATTACK);
}

// �X�V����
void PlayerAttacking_1::Update(const float& elapsedTime)
{
	m_totalSeconds += elapsedTime;

	// ���Ԃ��v�����A��莞�Ԍo�߂ŃX�e�[�g��J��
	m_player->TimeComparison(m_totalSeconds, Player::APPLIED_ATTACK_TIME, m_player->GetPlayerIdlingState(), elapsedTime);
}


// �L�[����
void PlayerAttacking_1::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	if (key == DirectX::Keyboard::X			&& m_totalSeconds >= Player::X_COOL_TIME)	m_player->ChangeState(m_player->GetPlayerAttackingState2());
	if (key == DirectX::Keyboard::LeftShift	&& m_totalSeconds >= Player::X_COOL_TIME)	m_player->ChangeState(m_player->GetPlayerDodgingState());
}

void PlayerAttacking_1::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}


// ����X�V����
void PlayerAttacking_1::PostUpdate()
{

	// ������U����ԂɕύX
	m_player->GetPlayScene()->GetSword()->ChangeState(
		m_player->GetPlayScene()->GetSword()->GetIdlingState()
	);
}


// �`�揈��
void PlayerAttacking_1::Render(
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

#ifdef _DEBUG
	// �f�o�b�O�����uDebugString�v�ŕ\������
	auto debugString = resources->GetDebugString();
	debugString->AddString("");
#endif // _DEBUG
}


// �I������
void PlayerAttacking_1::Finalize()
{
}
