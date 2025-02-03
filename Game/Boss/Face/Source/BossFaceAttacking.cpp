// -------------------------------------- //
//
// �G�̊獷�� : �ҋ@
//
// -------------------------------------- //
#include "pch.h"
#include "Game/CommonResources.h"
#include "Game/GameResources.h"
#include "DeviceResources.h"
#include "../../Boss.h"
#include "../Header/BossFaceAttacking.h"

// --------------------
// �R���X�g���N�^
// --------------------
BossFaceAttacking::BossFaceAttacking(Boss* Boss)
	: m_Boss(Boss)
{
	// ���f���擾
	m_model = GameResources::GetInstance()->GetModel("bossAttackingFace");
}

// --------------------
// �f�X�g���N�^
// --------------------
BossFaceAttacking::~BossFaceAttacking()
{
}

// --------------------
// �`�揈��
// --------------------
void BossFaceAttacking::DrawFace(
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