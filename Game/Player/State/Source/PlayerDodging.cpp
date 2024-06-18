#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"

#include "Game/Player/Player.h"
#include "Game/Player/State/Header/PlayerDodging.h"

// �Œ�l
const float PlayerDodging::DODGING_TIME = 1.0f;
const float PlayerDodging::DODGE_FUNCTION = 0.9f;
const int   PlayerDodging::TRUNCATION_DIGIT = 5;

// �R���X�g���N�^
PlayerDodging::PlayerDodging(Player* player)
	:
	 m_player(player)
	,m_totalSeconds()
	,m_direction()
{
}

// �f�X�g���N�^
PlayerDodging::~PlayerDodging()
{
}


// ����������
void PlayerDodging::Initialize(DirectX::Model* model)
{
	// ���f�����擾����
	m_model = model;
}

// ���O�X�V����
void PlayerDodging::PreUpdate()
{
	// �o�ߎ��Ԃ̏�����
	m_totalSeconds = 0.f;

	m_velocity = m_player->GetVelocity() * 4.f;
}

// �X�V����
void PlayerDodging::Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	// �X�e�[�g�̌o�ߎ��Ԃ��L�^����
	m_totalSeconds += elapsedTime;

	if (m_totalSeconds >= DODGING_TIME)
	{
		// ��莞�Ԍo�ߌ�ɃV�[����ҋ@�ɖ߂�
		m_player->ChangeState(m_player->GetPlayerIdlingState());
		return;
	}

	// �v���C���[�̉�]���擾
	Matrix angle = Matrix::CreateRotationY(-m_player->GetAngle());
	// ���x��ݒ�
	m_velocity *= DODGE_FUNCTION;



	// �ړ��ʂ����W�ɔ��f�����Ȃ�����W���ړ�������B
	parentPos -=Vector3::Transform(m_velocity,angle);
}


// ����X�V����
void PlayerDodging::PostUpdate()
{
	// �C���_������΂����ɋL��
}


// �`�揈��
void PlayerDodging::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
	)
{
	// �R�������\�[�X���擾����
	CommonResources* resources = CommonResources::GetInstance();

	// �f�o�b�O�����̕`��
	auto debugString = resources->GetDebugString();
	debugString->AddString("PlayerDodging");
	debugString->AddString("Dodging::Direction = [ %f, %f, %f ]", m_direction.x, m_direction.y, m_direction.z);

}


// �I������
void PlayerDodging::Finalize()
{
}
