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

// --------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="Boss">ボスのポインタ</param>
// --------------------------------------------------------
BossFaceIdling::BossFaceIdling(Boss* Boss)
	: m_Boss(Boss)
{
	// モデル取得
	m_model = GameResources::GetInstance()->GetModel("bossIdlingFace");
}

// --------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// --------------------------------------------------------
BossFaceIdling::~BossFaceIdling()
{
}

// --------------------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
/// <param name="mat">ワールド行列</param>
/// <param name="view">ビュー行列</param>
/// <param name="proj">プロジェクション行列</param>
// --------------------------------------------------------
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