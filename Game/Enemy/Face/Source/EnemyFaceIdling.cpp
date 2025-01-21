// -------------------------------------- //
//
// 敵の顔差分 : 待機
//
// -------------------------------------- //

#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Game/GameResources.h"
#include "../../Enemy.h"
#include "../Header/EnemyFaceIdling.h"



// --------------------
// コンストラクタ
// --------------------
EnemyFaceIdling::EnemyFaceIdling(Enemy* enemy)
	: m_enemy(enemy)
{
	CreateModel();
}

// --------------------
// デストラクタ
// --------------------
EnemyFaceIdling::~EnemyFaceIdling()
{
}


// --------------------
// モデルの生成処理
// --------------------
void EnemyFaceIdling::CreateModel()
{
	// リソースの取得
	CommonResources* resources = CommonResources::GetInstance();
	auto device = resources->GetDeviceResources()->GetD3DDevice();

	// モデル取得
	m_model = GameResources::GetInstance()->GetModel("bossIdlingFace");
}


// --------------------
// デストラクタ
// --------------------
void EnemyFaceIdling::DrawFace(
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