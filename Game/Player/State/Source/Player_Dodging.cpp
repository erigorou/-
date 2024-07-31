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
const int   PlayerDodging::TRUNCATION_DIGIT = 3;

// �R���X�g���N�^
PlayerDodging::PlayerDodging(Player* player)
	:
	 m_player(player)
	,m_model(nullptr)
	,m_totalSeconds(0.0f)
	,m_direction(0.0f, 0.0f, 0.0f)
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
	// �̂̓����蔻��̐���
	m_boundingSphereBody = DirectX::BoundingSphere();
	// �̂̓����蔻��̃T�C�Y����W��ݒ�
	m_boundingSphereBody.Radius = Player::PLAYER_SCALE * 12.f;
}

// ���O�X�V����
void PlayerDodging::PreUpdate()
{
	// �o�ߎ��Ԃ̏�����
	m_totalSeconds = 0.f;
	// �������������擾
	m_velocity = m_player->GetVelocity() * 4.f;
	// �����蔻����v���C���[�̈ʒu�ɏC��
	m_boundingSphereBody.Center = m_player->GetPosition();
}

// �X�V����
void PlayerDodging::Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

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

	// ���ߍ��ݗʂ̌v�Z��������ɂ���𔽉f������
	parentPos += CalculatingPushBack();
	m_boundingSphereBody.Center = parentPos;
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




	// �R�������\�[�X���擾����
	CommonResources* resources = CommonResources::GetInstance();

	// �f�o�b�O�����̕`��
	auto debugString = resources->GetDebugString();
	debugString->AddString("PlayerDodging");

}


// �I������
void PlayerDodging::Finalize()
{
}


// �̂ɓ��������Ƃ��ɉ����߂�������
DirectX::SimpleMath::Vector3 PlayerDodging::CalculatingPushBack()
{
	// �v���C���[����x�ϊ�����
	auto player = dynamic_cast<Player*>(m_player);
	// �{�f�B���擾						�v���C���[ �� �V�[�� �� �G�l�~�[ �� ���݂̃X�e�[�g �� �̂̓����蔻��
	DirectX::BoundingSphere enemyBody = player->GetPlayScene()->GetEnemy()->GetCurrentState()->GetBoundingSphereBody();
	// �����߂��ʂ̌v��
	return Math::pushBack_BoundingSphere(m_boundingSphereBody, enemyBody);
}