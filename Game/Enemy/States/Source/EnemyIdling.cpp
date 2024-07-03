#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Enemy/States/Header/EnemyIdling.h"


// �R���X�g���N�^
EnemyIdling::EnemyIdling(Enemy* enemy)
	:
	m_angle(0.f),
	m_enemy(enemy),
	m_totalSeconds()
{
}


// �f�X�g���N�^
EnemyIdling::~EnemyIdling()
{
}


// ����������
void EnemyIdling::Initialize(DirectX::Model* model)
{
	// ���f���̎擾
	m_model = model;

	// �̂̓����蔻��̐���
	m_boundingSphereBody = DirectX::BoundingSphere();
	// �̂̓����蔻��̃T�C�Y����W��ݒ�
	m_boundingSphereBody.Radius = Enemy::ENEMY_SCALE * 12.f;
}


// ���O�X�V����
void EnemyIdling::PreUpdate()
{
	// �o�ߎ��Ԃ�������
	m_totalSeconds = 0;
}


// �X�V����
void EnemyIdling::Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos)
{
	// elapsedTime�g��Ȃ����ǃG���[�o���Ȃ��ł˂��ĕ�
	UNREFERENCED_PARAMETER(elapsedTime);
	m_totalSeconds += elapsedTime;


	// �����Ƀr�w�C�r�A�c���[������

		// �G�ɋ߂Â�����
	using namespace DirectX::SimpleMath;
	// �v���C���[�̍��W���擾
	Vector3 playerPos = m_enemy->GetPlayScene()->GetPlayer()->GetPosition();
	// �G���猩���v���C���[�̈ʒu���v�Z����
	m_angle = Math::CalculationAngle(parentPos, playerPos);

	// �̂̋��E���̈ʒu���X�V
	m_boundingSphereBody.Center = parentPos;

	// ��]�p��ݒ肷��
	m_enemy->SetAngle(m_angle);

	if (m_totalSeconds >= 2.f)
	{
		m_enemy->ChangeState(m_enemy->GetEnemyApproaching());
	}
}


// �v���C���[�̑̂Ƃ̓����蔻����s��
void EnemyIdling::CheckHitPlayerBody()
{

}


// ����X�V����
void EnemyIdling::PostUpdate()
{
	// �C���_������΂����ɋL��
}


// �`�揈��
void EnemyIdling::Render(
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
	debugString->AddString("enemyAngle : %f", m_angle);
}


void EnemyIdling::Finalize()
{
}