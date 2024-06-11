#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"

#include "Game/Player/Player.h"
#include "Game/Player/State/PlayerIdling.h"

// �R���X�g���N�^
PlayerIdling::PlayerIdling(Player* player)
	:
	 m_player(player)
	,m_totalSeconds()
{
}

// �f�X�g���N�^
PlayerIdling::~PlayerIdling()
{
}


// ����������
void PlayerIdling::Initialize()
{
}

// ���O�X�V����
void PlayerIdling::PreUpdate()
{
	// �o�ߎ��Ԃ̏�����
	m_totalSeconds = 0.f;
}

// �X�V����
void PlayerIdling::Update(const float& elapsedTime)
{
	// �q�̃X�e�[�g�ɂȂ��Ă���̎��Ԃ��v��
	m_totalSeconds += elapsedTime;

	// �ڍs�X�V����������
}


// ����X�V����
void PlayerIdling::PostUpdate()
{
	// �C���_������΂����ɋL��
}


// �`�揈��
void PlayerIdling::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
	)
{

}


// �I������
void PlayerIdling::Finalize()
{
}

