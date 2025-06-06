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
#include "Libraries/MyLib/ThreadedRenderer/ThreadedRenderer.h"


/// <summary>
/// コンストラクタ
/// </summary>
Sign::Sign()
{
	// マルチスレッドに登録
	ThreadedRenderer::GetInstance()->RegisterRenderable(this);

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
	// マルチスレッドから登録解除
	ThreadedRenderer::GetInstance()->UnregisterRenderable(this);
}


/// <summary>
/// 描画コマンドを記録する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">プロジェクション行列</param>
/// <param name="deferredContext">遅延コンテキスト</param>
void Sign::RecordRenderCommands(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, ID3D11DeviceContext* deferredContext)
{
	CommonResources* resources = CommonResources::GetInstance();
	auto states = resources->GetCommonStates();

	// モデルのエフェクト情報を更新する処理
	m_signModel->UpdateEffects([](DirectX::IEffect* effect)
		{
			// ベーシックエフェクトを設定する
			DirectX::BasicEffect* basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			if (basicEffect)
			{
				// 個別のライトをすべて無効化する
				basicEffect->SetLightEnabled(0, false);
				basicEffect->SetLightEnabled(1, false);
				basicEffect->SetLightEnabled(2, false);

				// モデルを自発光させる
				basicEffect->SetEmissiveColor(DirectX::Colors::White);
			}
		}
	);

	// 初期状態のワールドマトリックスを設定
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::Identity;

	// 大きさ変更
	world *= DirectX::SimpleMath::Matrix::CreateScale(SIGN_SCALE);

	// 回転
	world *= DirectX::SimpleMath::Matrix::CreateRotationY(SIGN_ROTATE);

	// 移動
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(SIGN_POSITION);

	// 描画
	m_signModel->Draw(deferredContext, *states, world, view, proj);
}
