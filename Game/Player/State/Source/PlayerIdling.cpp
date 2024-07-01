#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"

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


// �̓��m�̓����蔻��
bool PlayerIdling::CheckBoundingSphereCollision()
{
	// �̂̓����������ǂ����̃t���O
	bool hit = false;

	auto player = dynamic_cast<Player*>(m_player);
	// �{�f�B���擾						�v���C���[ �� �V�[�� �� �G�l�~�[ �� ���݂̃X�e�[�g �� �̂̓����蔻��
	DirectX::BoundingSphere enemyBody = player->GetPlayScene()->GetEnemy()->GetCurrentState()->GetBoundingSphereBody();
	// �{�f�B���m�̓����蔻������s����
	if (m_boundingSphereBody.Intersects(enemyBody))
	{
		// �������Ă�Ȃ�True
		hit = true;
	}

	// �q�b�g�t���O�̒��g��Ԃ�
	return hit;
}


// �̓��m�œ��������ꍇ�̏���
void PlayerIdling::HitBody()
{
	using namespace DirectX::SimpleMath;

	// �v���C���[����x�ϊ�����
	auto player = dynamic_cast<Player*>(m_player);
	// �{�f�B���擾						�v���C���[ �� �V�[�� �� �G�l�~�[ �� ���݂̃X�e�[�g �� �̂̓����蔻��
	DirectX::BoundingSphere enemyBody = player->GetPlayScene()->GetEnemy()->GetCurrentState()->GetBoundingSphereBody();
	// �������ĂȂ��ꍇ�͑������^�[��
	if (!m_boundingSphereBody.Intersects(enemyBody))	return;

	// �Փ˔���@�v���C���[�������߂����--[====================>

	// �v���C���[�̒��S�ƓG�̒��S�Ƃ̍����x�N�g��
	Vector3 diffVec = m_boundingSphereBody.Center - enemyBody.Center;
	// �v���C���[�̒��S�ƓG�̒��S�Ƃ̋������擾
	float diffLength = diffVec.Length();
	// �v���C���[�ƓG�̔��a�̍��v
	float sumLength = m_boundingSphereBody.Radius + enemyBody.Radius;
	//
}