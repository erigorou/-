#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"

#include "Game/Player/Player.h"
#include "Game/Player/State/Header/PlayerIdling.h"

// �R���X�g���N�^
PlayerIdling::PlayerIdling(Player* player)
	:
	 m_player(player)
	,m_totalSeconds()
	,m_model()
{
}


// �f�X�g���N�^
PlayerIdling::~PlayerIdling()
{
}


// ����������
void PlayerIdling::Initialize(DirectX::Model* model)
{
	// ���f�����擾����
	m_model = model;
	// �̂̓����蔻��̐���
	m_boundingSphereBody = DirectX::BoundingSphere();
	// �̂̓����蔻��̃T�C�Y����W��ݒ�
	m_boundingSphereBody.Radius = 0.3f;
}


// ���O�X�V����
void PlayerIdling::PreUpdate()
{
	// �o�ߎ��Ԃ̏�����
	m_totalSeconds = 0.f;
}


// �X�V����
void PlayerIdling::Update(const float& elapsedTime,  DirectX::SimpleMath::Vector3& parentPos)
{
	// elapsedTime�g��Ȃ����ǃG���[�o���Ȃ��ł˂��ĕ�
	UNREFERENCED_PARAMETER(elapsedTime);

	// �L�[���͂��󂯕t����B
	DirectX::Keyboard::State keyboardState = DirectX::Keyboard::Get().GetState();
	// �v���C���[�̈ړ�
	m_player->MovePlayer();
	// ���V�t�g�̓��́@&& �ړ����Ȃ�
	if (keyboardState.LeftShift && m_player->GetVelocity() != DirectX::SimpleMath::Vector3::Zero)
	{
		m_player->ChangeState(m_player->GetPlayerDodgingState());
	}
	// X�̓���
	if (keyboardState.X)
	{
		m_player->ChangeState(m_player->GetPlayerAttackingState());
	}
	// �����蔻��̍��W���X�V
	m_boundingSphereBody.Center = parentPos;
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
	// �R�������\�[�X���擾����
	CommonResources* resources = CommonResources::GetInstance();

	//// �f�o�b�O�����̕`��
	//auto debugString = resources->GetDebugString();
	//debugString->AddString("PlayerIdling");
}


// �I������
void PlayerIdling::Finalize()
{
}

