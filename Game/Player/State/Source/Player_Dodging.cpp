#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Player/State/Header/Player_Dodging.h"

// �Œ�l
const float PlayerDodging::DODGING_TIME = 0.5f;
const float PlayerDodging::DODGE_FUNCTION = 0.9f;
const int   PlayerDodging::TRUNCATION_DIGIT = 4;

// �R���X�g���N�^
PlayerDodging::PlayerDodging(Player* player)
	:
	 m_player(player)
	,m_model(nullptr)
	,m_totalSeconds(0.0f)
	,m_inputVelocity(0.0f, 0.0f, 0.0f)
	,m_finishTime(0.0f)
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

	m_velocity = DirectX::SimpleMath::Vector3::Forward * DODGING_SPEED;

	m_position = m_player->GetPosition();
}

// �X�V����
void PlayerDodging::Update(const float& elapsedTime)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	// �v���C���[�̍��W���擾
	Vector3 parentPos = m_player->GetPosition();

	// �X�e�[�g�J�n���玞�Ԃ��v���A��莞�Ԃŕʂ�State�֑J�ڂ�����
	m_player->TimeComparison(m_totalSeconds, DODGING_TIME, m_player->GetPlayerIdlingState(), elapsedTime);

	// �v���C���[�̉�]���擾
	Matrix angle = Matrix::CreateRotationY(-m_player->GetAngle());
	// ���x��ݒ�
	m_velocity *= DODGE_FUNCTION;
	// �[����������΂��B
	m_velocity = Math::truncate_vector(m_velocity, TRUNCATION_DIGIT);
	// �ړ��ʂ����W�ɔ��f�����Ȃ�����W���ړ�������B
	parentPos +=Vector3::Transform(m_velocity,angle);

	// �v���C���[�̍��W���X�V
	m_player->SetPosition(parentPos);
}

// �L�[����
void PlayerDodging::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}

void PlayerDodging::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
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
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(states);
	UNREFERENCED_PARAMETER(view);
	UNREFERENCED_PARAMETER(projection);
	UNREFERENCED_PARAMETER(m_model);

#ifdef _DEBUG
	// �R�������\�[�X���擾����
	CommonResources* resources = CommonResources::GetInstance();
	// �f�o�b�O�����̕`��
	auto debugString = resources->GetDebugString();
	debugString->AddString("PlayerDodging");
#endif // _DEBUG
}


// �I������
void PlayerDodging::Finalize()
{
}
