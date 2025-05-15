// -------------------------
//
// 　天球に関するクラス
//
// --------------------------

#include "pch.h"

#include "SkySphere.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/ThreadedRenderer/ThreadedRenderer.h"

const float SkySphere::SKYSPHERE_SCALE = 100.f;

// -------------------------------
//  コンストラクタ
// -------------------------------
SkySphere::SkySphere()
	:
	m_skySphereModel()
{
	// マルチスレッドに自身を登録
	auto threadedRenderer = ThreadedRenderer::GetInstance();
	threadedRenderer->RegisterRenderable(this);
}

// -------------------------------
//  デストラクタ
// -------------------------------
SkySphere::~SkySphere()
{
	// 描画コマンドの削除
	auto threadedRenderer = ThreadedRenderer::GetInstance();
	threadedRenderer->UnregisterRenderable(this);
}

// -------------------------------
//  ロード処理
// -------------------------------
void SkySphere::LoadSkySphereModel()
{
	auto commonResources = CommonResources::GetInstance();
	auto device = commonResources->GetDeviceResources()->GetD3DDevice();

	// モデルを読み込む準備
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models/ddsFile");

	// モデルを読み込む
	m_skySphereModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Stage/SkySphere/skySphere.cmo", *fx);
}

// -------------------------------
//  描画処理
// -------------------------------
void SkySphere::DrawSkySphere(
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix projection
)
{
	using namespace DirectX;

	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	// モデルのエフェクト情報を更新する処理
	m_skySphereModel->UpdateEffects([](DirectX::IEffect* effect)
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
	// サイズを調整する
	world *= SimpleMath::Matrix::CreateScale(SKYSPHERE_SCALE);

	// 天球を描画する
	m_skySphereModel->Draw(context, *states, world, view, projection);
}


/// <summary>
/// 描画コマンドを登録する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">プロジェクション行列</param>
/// <param name="deferredContext">ディファードコンテキスト</param>
void SkySphere::RecordRenderCommands(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj,
	ID3D11DeviceContext* deferredContext)
{
	CommonResources* resources = CommonResources::GetInstance();
	auto states = resources->GetCommonStates();

	// モデルのエフェクト情報を更新する処理
	m_skySphereModel->UpdateEffects([](DirectX::IEffect* effect)
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
	// サイズを調整する
	world *= DirectX::SimpleMath::Matrix::CreateScale(SKYSPHERE_SCALE);

	// 天球を描画する
	m_skySphereModel->Draw(deferredContext, *states, world, view, proj);
}
