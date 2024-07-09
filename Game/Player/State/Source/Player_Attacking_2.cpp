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

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Player/State/Header/Player_Attacking_2.h"

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

	// �������Ƃ�������������L�[�{�[�h
	DirectX::Keyboard::State keyboard = DirectX::Keyboard::Get().GetState();
	DirectX::Keyboard::KeyboardStateTracker tracker;
	tracker.Update(keyboard);


	// 2��ڂ̍U�����ɍU���{�^��������
	if (tracker.IsKeyPressed(DirectX::Keyboard::D2))
	{
		// 3��ڂ̍U���ɃX�e�[�g��ύX����
		m_player->ChangeState(m_player->GetPlayerAttackingState3());
	}

	// ���V�t�g�ŉ��
	if (tracker.IsKeyPressed(DirectX::Keyboard::LeftShift))
	{
		m_player->ChangeState(m_player->GetPlayerDodgingState());
	}

	// ���Ԃ��v�����A��莞�Ԍo�߂ŃX�e�[�g��J��
	m_player->TimeComparison(m_totalSeconds, Player::APPLIED_ATTACK_TIME, m_player->GetPlayerIdlingState(), elapsedTime);
	
	// �߂荞�ݗʂ��v�Z���A��������W�ɔ��f������i�����߂��j
	parentPos += CalculatingPushBack();
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
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(states);
	UNREFERENCED_PARAMETER(view);
	UNREFERENCED_PARAMETER(projection);
	UNREFERENCED_PARAMETER(m_model);

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


// �̂ɓ��������Ƃ��ɉ����߂�������
DirectX::SimpleMath::Vector3 PlayerAttacking_2::CalculatingPushBack()
{
	// �v���C���[����x�ϊ�����
	auto player = dynamic_cast<Player*>(m_player);
	// �{�f�B���擾						�v���C���[ �� �V�[�� �� �G�l�~�[ �� ���݂̃X�e�[�g �� �̂̓����蔻��
	DirectX::BoundingSphere enemyBody = player->GetPlayScene()->GetEnemy()->GetCurrentState()->GetBoundingSphereBody();
	// �����߂��ʂ̌v��
	return Math::pushBack_BoundingSphere(m_boundingSphereBody, enemyBody);
}