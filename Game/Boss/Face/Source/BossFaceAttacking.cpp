// -------------------------------------- //
//
// 敵の顔差分 : 待機
//
// -------------------------------------- //
#include "pch.h"
#include "Game/CommonResources.h"
#include "Game/GameResources.h"
#include "DeviceResources.h"
#include "../../Boss.h"
#include "../Header/BossFaceAttacking.h"

// --------------------
// コンストラクタ
// --------------------
BossFaceAttacking::BossFaceAttacking(Boss* Boss)
	: m_Boss(Boss)
{
	// モデル取得
	m_model = GameResources::GetInstance()->GetModel("bossAttackingFace");
}

// --------------------
// デストラクタ
// --------------------
BossFaceAttacking::~BossFaceAttacking()
{
}

// --------------------
// 描画処理
// --------------------
void BossFaceAttacking::DrawFace(
	const DirectX::SimpleMath::Matrix mat,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj
)
{
	// リソースの取得
	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();

	// モデルの描画
	m_model->Draw(context, *resources->GetCommonStates(), mat, view, proj);
}