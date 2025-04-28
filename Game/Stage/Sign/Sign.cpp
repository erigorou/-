// ---------------------------------------------------------
//
// 名前：Sign.h
// 説明：看板クラス
// 作者：池田桜輔
//
// ---------------------------------------------------------

// インクルード
#include "pch.h"
#include "Sign.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"


/// <summary>
/// コンストラクタ
/// </summary>
Sign::Sign()
{
	auto commonResources = CommonResources::GetInstance();
	auto device = commonResources->GetDeviceResources()->GetD3DDevice();

	// モデルを読み込む準備
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models/ddsFile");

	// モデルを読み込む
	m_signModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Select/SelectEscapeSign.cmo", *fx);
}


/// <summary>
/// デストラクタ
/// </summary>
Sign::~Sign()
{
}



void Sign::DrawSign(
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix projection
)
{
	using namespace DirectX;

	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	// モデルのエフェクト情報を更新する処理
	m_signModel->UpdateEffects([](DirectX::IEffect* effect)
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

	// 大きさ変更
	world *= SimpleMath::Matrix::CreateScale(SIGN_SCALE);

	// 回転
	world *= SimpleMath::Matrix::CreateRotationY(SIGN_ROTATE);

	// 移動
	world *= SimpleMath::Matrix::CreateTranslation(SIGN_POSITION);

	// 描画
	m_signModel->Draw(context, *states, world, view, projection);
}
