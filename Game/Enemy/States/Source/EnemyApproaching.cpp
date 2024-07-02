#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/States/Header/EnemyApproaching.h"


// �R���X�g���N�^
EnemyApproaching::EnemyApproaching(Enemy* enemy)
	:
	m_enemy(enemy),
	m_totalSeconds()
{
}


// �f�X�g���N�^
EnemyApproaching::~EnemyApproaching()
{
}


// ����������
void EnemyApproaching::Initialize(DirectX::Model* model)
{
	// ���f���̎擾
	m_model = model;

	// �̂̓����蔻��̐���
	m_boundingSphereBody = DirectX::BoundingSphere();
	// �̂̓����蔻��̃T�C�Y����W��ݒ�
	m_boundingSphereBody.Radius = Enemy::ENEMY_SCALE * 12.f;

	// �U�ꕝ
	m_amplitude = 1.f;
	// ���g��
	m_frequency = 2.f;
}


// ���O�X�V����
void EnemyApproaching::PreUpdate()
{
	// �o�ߎ��Ԃ�������
	m_totalSeconds = 0.f;
}


// �X�V����
void EnemyApproaching::Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos)
{
	// elapsedTime�g��Ȃ����ǃG���[�o���Ȃ��ł˂��ĕ�
	UNREFERENCED_PARAMETER(elapsedTime);
	
	m_totalSeconds += elapsedTime;

	// �T�C���g�̌v�Z(�㉺�ړ�)
	parentPos.y = Math::CalculatingSinWave(m_totalSeconds, m_amplitude, m_frequency);

	// �G�ɋ߂Â�
	using namespace DirectX::SimpleMath;
	Vector3 enemyPosition = m_enemy->GetPlayScene()->GetPlayer()->GetPosition();


	// �̂̋��E���̈ʒu���X�V
	m_boundingSphereBody.Center = parentPos;

	// 2�b�o�߂őҋ@���[�V�����ɕύX
	if (m_totalSeconds >= 2.f)
	{
		m_enemy->ChangeState(m_enemy->GetEnemyIdling());
	}
}


// �v���C���[�̑̂Ƃ̓����蔻����s��
void EnemyApproaching::CheckHitPlayerBody()
{
}


// ����X�V����
void EnemyApproaching::PostUpdate()
{
	// �C���_������΂����ɋL��
}


// �`�揈��
void EnemyApproaching::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
	)
{
	UNREFERENCED_PARAMETER(context, states, view, projection, m_model);



	// ���\�[�X�̎擾
	CommonResources* resources = CommonResources::GetInstance();
	// �f�o�b�O�����uDebugString�v�ŕ\������
	auto debugString = resources->GetDebugString();
}


void EnemyApproaching::Finalize()
{
}