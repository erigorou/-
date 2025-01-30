// -------------------------------------- //
//
// �G�̊獷�� : �ҋ@
//
// -------------------------------------- //

#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Game/GameResources.h"
#include "../../Enemy.h"
#include "../Header/EnemyFaceIdling.h"


// --------------------
// �R���X�g���N�^
// --------------------
EnemyFaceIdling::EnemyFaceIdling(Enemy* enemy)
	: m_enemy(enemy)
{
	// ���f���擾
	m_model = GameResources::GetInstance()->GetModel("bossIdlingFace");
}

// --------------------
// �f�X�g���N�^
// --------------------
EnemyFaceIdling::~EnemyFaceIdling()
{
}


// --------------------
// ��̕`��
// --------------------
void EnemyFaceIdling::DrawFace(
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