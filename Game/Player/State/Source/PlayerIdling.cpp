#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Enemy/Enemy.h"
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
	m_boundingSphereBody.Radius = Player::PLAYER_SCALE * 12.f;
}


// ���O�X�V����
void PlayerIdling::PreUpdate()
{
	// �o�ߎ��Ԃ̏�����
	m_totalSeconds = 0.f;
	// �����蔻����v���C���[�̈ʒu�ɏC��
	m_boundingSphereBody.Center = m_player->GetPosition();
}


// �X�V����
void PlayerIdling::Update(const float& elapsedTime,  DirectX::SimpleMath::Vector3& parentPos)
{
	// elapsedTime�g��Ȃ����ǃG���[�o���Ȃ��ł˂��ĕ�
	UNREFERENCED_PARAMETER(elapsedTime);

	// �L�[�{�[�h�̓��͂��擾����
	m_keyboardState = DirectX::Keyboard::Get().GetState();

	// �v���C���[�̈ړ�
	m_player->MovePlayer();

	// ���V�t�g�̓��́@&& �ړ����Ȃ�
	if (m_keyboardState.LeftShift && m_player->GetVelocity() != DirectX::SimpleMath::Vector3::Zero)
	{
		m_player->ChangeState(m_player->GetPlayerDodgingState());
	}
	// X�̓���
	if (m_keyboardState.X)
	{
		m_player->ChangeState(m_player->GetPlayerAttackingState1());
	}


	// ���ߍ��ݗʂ̌v�Z��������ɂ���𔽉f������
	parentPos += CalculatingPushBack();
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
}


// �I������
void PlayerIdling::Finalize()
{
}



// �̂ɓ��������Ƃ��ɉ����߂�������
DirectX::SimpleMath::Vector3 PlayerIdling::CalculatingPushBack()
{
	// �v���C���[����x�ϊ�����
	auto player = dynamic_cast<Player*>(m_player);
	// �{�f�B���擾						�v���C���[ �� �V�[�� �� �G�l�~�[ �� ���݂̃X�e�[�g �� �̂̓����蔻��
	DirectX::BoundingSphere enemyBody = player->GetPlayScene()->GetEnemy()->GetCurrentState()->GetBoundingSphereBody();
	// �����߂��ʂ̌v��
	return Math::pushBack_BoundingSphere(m_boundingSphereBody, enemyBody);
}