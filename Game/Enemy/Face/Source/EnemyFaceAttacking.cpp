// -------------------------------------- //
//
// 敵の顔差分 : 待機
//
// -------------------------------------- //
#include "pch.h"
#include "Game/CommonResources.h"
#include "Game/GameResources.h"
#include "DeviceResources.h"
#include "../../Enemy.h"
#include "../Header/EnemyFaceAttacking.h"


// --------------------
// コンストラクタ
// --------------------
EnemyFaceAttacking::EnemyFaceAttacking(Enemy* enemy)
	: m_enemy(enemy)
{
	// モデル取得
	m_model = GameResources::GetInstance()->GetModel("bossAttackingFace");
}

// --------------------
// デストラクタ
// --------------------
EnemyFaceAttacking::~EnemyFaceAttacking()
{
}


// --------------------
// 描画処理
// --------------------
void EnemyFaceAttacking::DrawFace(
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