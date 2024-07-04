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

#include "Game/Player/Player.h"
#include "Game/Player/State/Header/PlayerAttacking_2.h"

// �R���X�g���N�^
PlayerAttacking_2::PlayerAttacking_2(Player* player)
	:
	 m_player(player)
	,m_totalSeconds()
	,m_model()
	,m_boundingSphereBody()
{
}

// �f�X�g���N�^
PlayerAttacking_2::~PlayerAttacking_2()
{
}


// ����������
void PlayerAttacking_2::Initialize(DirectX::Model* model)
{
	// ���f�����擾����
	m_model = model;
	// �̂̋��E�����쐬
	m_boundingSphereBody = DirectX::BoundingSphere();
	m_boundingSphereBody.Radius = Player::PLAYER_SCALE * 12.f;


}

// ���O�X�V����
void PlayerAttacking_2::PreUpdate()
{
	// �o�ߎ��Ԃ̏�����
	m_totalSeconds = 0.f;
}

// �X�V����
void PlayerAttacking_2::Update(const float& elapsedTime,  DirectX::SimpleMath::Vector3& parentPos)
{
	// parentPos�g��Ȃ����ǃG���[�o���Ȃ��ł˂��ĕ�
	UNREFERENCED_PARAMETER(parentPos);

	// �L�[�{�[�h�̓��͂��擾����
	m_keyboardState = DirectX::Keyboard::Get().GetState();
	// �g���b�J�[�̎���
	DirectX::Keyboard::KeyboardStateTracker tracker;
	tracker.Update(m_keyboardState);

	// 2��ڂ̍U�����ɍU���{�^��������
	if (tracker.IsKeyPressed(DirectX::Keyboard::X))
	{
		// 3��ڂ̍U���ɃX�e�[�g��ύX����
		m_player->ChangeState(m_player->GetPlayerAttackingState3());
	}

	// ���Ԃ��v�����A��莞�Ԍo�߂ŃX�e�[�g��J��
	m_player->TimeComparison(m_totalSeconds, Player::APPLIED_ATTACK_TIME, m_player->GetPlayerIdlingState(), elapsedTime);
	// �̂̋��E���̈ʒu���X�V
	m_boundingSphereBody.Center = parentPos;


}


// ����X�V����
void PlayerAttacking_2::PostUpdate()
{
	// �C���_������΂����ɋL��
}


// �`�揈��
void PlayerAttacking_2::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
	)
{
	UNREFERENCED_PARAMETER(context, states, view, projection, m_model);
	// �R�������\�[�X���擾����
	CommonResources* resources = CommonResources::GetInstance();


	// �f�o�b�O�����uDebugString�v�ŕ\������
	auto debugString = resources->GetDebugString();
	debugString->AddString("Attack 2");
}


// �I������
void PlayerAttacking_2::Finalize()
{
}
