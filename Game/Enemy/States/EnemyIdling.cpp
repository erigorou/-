#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/States/EnemyIdling.h"


// �R���X�g���N�^
EnemyIdling::EnemyIdling(Enemy* enemy)
	:
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

	// �����Ƀr�w�C�r�A�c���[������

	// �̂̋��E���̈ʒu���X�V
	m_boundingSphereBody.Center = parentPos;
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
}


void EnemyIdling::Finalize()
{
}