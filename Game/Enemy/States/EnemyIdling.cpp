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
void EnemyIdling::Initialize()
{
}


// ���O�X�V����
void EnemyIdling::PreUpdate()
{
	// �o�ߎ��Ԃ�������
	m_totalSeconds = 0;
}


// �X�V����
void EnemyIdling::Update(const float& elapsedTime)
{
	// ���̃X�e�[�g�ɂȂ��Ă���̎��Ԃ��v��
	m_totalSeconds += elapsedTime;



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
	// ���\�[�X�̎擾
	CommonResources* resources = CommonResources::GetInstance();

	// �f�o�b�O�����uDebugString�v�ŕ\������
	auto debugString = resources->GetDebugString();
	debugString->AddString("State:EnemyIdling");
}


void EnemyIdling::Finalize()
{
}