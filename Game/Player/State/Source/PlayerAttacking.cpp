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
#include "Game/Player/State/Header/PlayerAttacking.h"

// �A�^�b�N���鎞��
const float PlayerAttacking::ATTACKING_TIME = 1.0f;

// �R���X�g���N�^
PlayerAttacking::PlayerAttacking(Player* player)
	:
	 m_player(player)
	,m_totalSeconds()
	,m_model()
{
}

// �f�X�g���N�^
PlayerAttacking::~PlayerAttacking()
{
}


// ����������
void PlayerAttacking::Initialize(DirectX::Model* model)
{
	// ���f�����擾����
	m_model = model;
}

// ���O�X�V����
void PlayerAttacking::PreUpdate()
{
	// �o�ߎ��Ԃ̏�����
	m_totalSeconds = 0.f;
}

// �X�V����
void PlayerAttacking::Update(const float& elapsedTime,  DirectX::SimpleMath::Vector3& parentPos)
{
	// parentPos�g��Ȃ����ǃG���[�o���Ȃ��ł˂��ĕ�
	UNREFERENCED_PARAMETER(parentPos);
	// ���Ԃ��v�����A��莞�Ԍo�߂ŃX�e�[�g��J��
	m_player->TimeComparison(m_totalSeconds, ATTACKING_TIME, m_player->GetPlayerIdlingState(), elapsedTime);
}


// ����X�V����
void PlayerAttacking::PostUpdate()
{
	// �C���_������΂����ɋL��
}


// �`�揈��
void PlayerAttacking::Render(
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
void PlayerAttacking::Finalize()
{
}
