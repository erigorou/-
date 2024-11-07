// -------------------------------------- //
//
// 敵の顔差分 : 待機
//
// -------------------------------------- //



#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "../../Enemy.h"

#include "../Header/EnemyFaceAttacking.h"


// --------------------
// コンストラクタ
// --------------------
EnemyFaceAttacking::EnemyFaceAttacking(Enemy* enemy)
	: m_enemy(enemy)
{
	CreateModel();
}

// --------------------
// デストラクタ
// --------------------
EnemyFaceAttacking::~EnemyFaceAttacking()
{
}


// --------------------
// モデルの生成処理
// --------------------
void EnemyFaceAttacking::CreateModel()
{
	// リソースの取得
	CommonResources* resources = CommonResources::GetInstance();
	auto device = resources->GetDeviceResources()->GetD3DDevice();

	// モデルを読み込む準備
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");

	// モデルの読み込み
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Oni/Face/AttackingFace.cmo", *fx);
}



// --------------------
// デストラクタ
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