// -------------------------------------- //
//
// �G�̊獷�� : �ҋ@
//
// -------------------------------------- //
#include "pch.h"
#include "Game/CommonResources.h"
#include "Game/GameResources.h"
#include "DeviceResources.h"
#include "../../Enemy.h"
#include "../Header/EnemyFaceAttacking.h"


// --------------------
// �R���X�g���N�^
// --------------------
EnemyFaceAttacking::EnemyFaceAttacking(Enemy* enemy)
	: m_enemy(enemy)
{
	// ���f���擾
	m_model = GameResources::GetInstance()->GetModel("bossAttackingFace");
}

// --------------------
// �f�X�g���N�^
// --------------------
EnemyFaceAttacking::~EnemyFaceAttacking()
{
}


// --------------------
// �`�揈��
// --------------------
void EnemyFaceAttacking::DrawFace(
	const DirectX::SimpleMath::Matrix mat,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj
)
{
	// ���\�[�X�̎擾
	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();

	// ���f���̕`��
	m_model->Draw(context, *resources->GetCommonStates(), mat, view, proj);
}