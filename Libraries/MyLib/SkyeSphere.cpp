// -------------------------
//
// 　天球に関するクラス
//
// --------------------------

#include "pch.h"

#include "Libraries/MyLib/SkyeSphere.h"
#include "Game/CommonResources.h"


// -------------------------------
//  コンストラクタ
// -------------------------------
SkyeSphere::SkyeSphere()
	:
	m_skyeSphereModel()
{
}


// -------------------------------
//  デストラクタ
// -------------------------------
SkyeSphere::~SkyeSphere()
{
}


// -------------------------------
//  ロード処理
// -------------------------------
void SkyeSphere::LoadSkyeSphereModel(ID3D11Device* device)
{
	auto commonResources = CommonResources::GetInstance();

	// モデルを読み込む準備
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");

	// モデルを読み込む
	m_skyeSphereModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/SkyeSphere.cmo", *fx);
}


// -------------------------------
//  描画処理
// -------------------------------
void SkyeSphere::DrawSkyeSphere(
	 ID3D11DeviceContext1* context
	, DirectX::DX11::CommonStates* states
	, DirectX::SimpleMath::Matrix view
	, DirectX::SimpleMath::Matrix projection
	)
{
	using namespace DirectX;

	// モデルのエフェクト情報を更新する処理
	m_skyeSphereModel->UpdateEffects([](DirectX::IEffect* effect)
		{
			// ベーシックエフェクトを設定する
			BasicEffect* basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				// 個別のライトをすべて無効化する
				basicEffect->SetLightEnabled(0, false);
				basicEffect->SetLightEnabled(1, false);
				basicEffect->SetLightEnabled(2, false);

				// モデルを自発光させる
				basicEffect->SetEmissiveColor(Colors::White);
			}
		}
	);

	// 初期状態のワールドマトリックスを設定
	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;

	// 天球を描画する
	m_skyeSphereModel->Draw(context, *states, world, view, projection);
}