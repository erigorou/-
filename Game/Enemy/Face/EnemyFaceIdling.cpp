// -------------------------------------- //
//
// 敵の顔差分 : 待機
//
// -------------------------------------- //



#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"

#include "EnemyFaceIdling.h"


// --------------------
// コンストラクタ
// --------------------
Enemy::EnemyFaceIdling::EnemyFaceIdling(Enemy* enemy)
	: m_enemy(enemy)
{
	CreateModel();
}

// --------------------
// デストラクタ
// --------------------
Enemy::EnemyFaceIdling::~EnemyFaceIdling()
{
}


// --------------------
// モデルの生成処理
// --------------------
void Enemy::EnemyFaceIdling::CreateModel()
{
	// リソースの取得
	CommonResources* resources = CommonResources::GetInstance();
	auto device = resources->GetDeviceResources()->GetD3DDevice();

	// モデルを読み込む準備
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");

	// モデルの読み込み
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Oni/Face/IdlingFace.cmo", *fx);
}



// --------------------
// デストラクタ
// --------------------
void Enemy::EnemyFaceIdling::DrawFace(
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