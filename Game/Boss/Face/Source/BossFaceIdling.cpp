// -------------------------------------- //
//
// 敵の顔差分 : 待機
//
// -------------------------------------- //

#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Game/GameResources.h"
#include "../../Boss.h"
#include "../Header/BossFaceIdling.h"

// --------------------
// コンストラクタ
// --------------------
BossFaceIdling::BossFaceIdling(Boss* Boss)
	: m_Boss(Boss)
{
	// モデル取得
	m_model = GameResources::GetInstance()->GetModel("bossIdlingFace");
}

// --------------------
// デストラクタ
// --------------------
BossFaceIdling::~BossFaceIdling()
{
}

// --------------------
// 顔の描画
// --------------------
void BossFaceIdling::DrawFace(
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