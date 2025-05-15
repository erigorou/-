// ** ---------------------------------------------------------**
//     敵モデルの死亡時・ダメージ時のエフェクトを管理するクラス
// ** ---------------------------------------------------------**

#include "pch.h"
#include "EnemyEffect.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/CommonResources.h"
#include "Game/GameResources.h"
#include "DeviceResources.h"

#include <windows.h>
#include <cstdio>

// ---------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// ---------------------------------------------
EnemyEffect::EnemyEffect()
	:
	m_totalTime{},
	m_deadShader{},
	m_buffer{},
	m_currentEffect{ ENEMY_EFFECT::NONE }
{
	// シェーダーの生成
	CreateShader();

	// 定数バッファの作成
	CreateConstBuffer();
}

// ---------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------------
EnemyEffect::~EnemyEffect()
{
}

// ---------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------
void EnemyEffect::Update(float elapsedTime)
{
	if (m_currentEffect == ENEMY_EFFECT::DEAD)
	{
		// 経過時間の記録
		m_totalTime = std::max(0.0f, (m_totalTime - elapsedTime));
	}
	else if (m_currentEffect == ENEMY_EFFECT::DAMAGE)
	{
		// 経過時間の記録
		m_totalTime = std::max(0.0f, (m_totalTime - elapsedTime));
	}
}

// ---------------------------------------------
/// <summary>
/// 敵の体パーツの描画処理
/// </summary>
/// <param name="model">描画するモデル</param>
/// <param name="world">ワールド行列</param>
/// <param name="view">ビュー行列</param>
/// <param name="proj">プロジェクション行列</param>
// ---------------------------------------------
void EnemyEffect::DrawWithEffect(
	DirectX::Model* model,
	const DirectX::SimpleMath::Matrix world,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj
)
{

	// 必要情報の取得
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = CommonResources::GetInstance()->GetCommonStates();

	// 定数バッファの更新
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	context->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	ConstBuffer* cb = static_cast<ConstBuffer*>(mappedResource.pData);

	// 定数バッファの更新処理
	UpdateConstBuffer(cb);

	context->Unmap(m_buffer.Get(), 0);

	ID3D11SamplerState* sampler[1] = { };
	context->PSSetSamplers(0, 1, sampler);

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture = GameResources::GetInstance()->GetTexture("noize");

	// モデルの描画
	model->Draw(context, *states, world, view, proj, false, [&]
		{
			// エフェクトの設定
			model->UpdateEffects([&](DirectX::IEffect* effect)
				{
					auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
					if (basicEffect)
					{
						basicEffect->SetLightingEnabled(true); // ライト有効化
						basicEffect->SetPerPixelLighting(true); // ピクセル単位のライティング有効化
						basicEffect->SetTextureEnabled(false); // テクスチャの無効化
						basicEffect->SetVertexColorEnabled(false); // 頂点カラーの無効化
						basicEffect->SetFogEnabled(false); // フォグの無効化
					}
				}
			);

			// シェーダーを当てるときは共通の計算を行う
			if (m_currentEffect != ENEMY_EFFECT::NONE)
			{
				// 定数バッファを設定
				ID3D11Buffer* cbuff = { m_buffer.Get() };
				// シェーダーにバッファを渡す
				context->PSSetConstantBuffers(1, 1, &cbuff);
				//	ピクセルシェーダにテクスチャを登録する。
				context->PSSetShaderResources(0, 1, texture.GetAddressOf());
				// ブレンドステートを設定
				context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xFFFFFFFF);
			}

			// ダメージを食らった時のエフェクトを適用
			if (m_currentEffect == ENEMY_EFFECT::DAMAGE)
			{
				// シェーダーの開始
				m_damageShader->BeginSharder(context);
			}
			// 死亡時のエフェクトを適用
			else if (m_currentEffect == ENEMY_EFFECT::DEAD)
			{
				// シェーダーの開始
				m_deadShader->BeginSharder(context);
			}
		}
	);
}


/// <summary>
/// 描画コマンドの登録
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">プロジェクション行列</param>
/// <param name="deferredContext">ディファードコンテキスト</param>
/// <param name="model">モデル</param>
/// <param name="world">ワールド行列</param>
void EnemyEffect::RecordRenderCommands(
	const DirectX::SimpleMath::Matrix& view, 
	const DirectX::SimpleMath::Matrix& proj,
	ID3D11DeviceContext* deferredContext,
	DirectX::Model* model,
	const DirectX::SimpleMath::Matrix world)
{
	// 必要情報の取得
	auto resources = CommonResources::GetInstance();
	auto states = resources->GetCommonStates();
	auto device = resources->GetDeviceResources()->GetD3DDevice();

	// モデルファクトリの生成
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models/ddsFile");

	std::unique_ptr<DirectX::Model> modelRef = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Goblin/goblin.cmo", *fx);

	// 定数バッファの更新
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	deferredContext->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	ConstBuffer* cb = static_cast<ConstBuffer*>(mappedResource.pData);

	// 定数バッファの更新処理
	UpdateConstBuffer(cb);

	// シェーダーの設定
		if (m_currentEffect != ENEMY_EFFECT::NONE)
		{
			// 頂点シェーダー・ピクセルシェーダーを設定（CustomShaderを使っている前提）
			if (m_currentEffect == ENEMY_EFFECT::DAMAGE)
			{
				m_damageShader->BeginSharder(deferredContext);
			}
			else if (m_currentEffect == ENEMY_EFFECT::DEAD)
			{
				m_deadShader->BeginSharder(deferredContext);
			}

			// 定数バッファをピクセルシェーダーにセット
		ID3D11Buffer* cbuff = m_buffer.Get();
		deferredContext->PSSetConstantBuffers(1, 1, &cbuff);

		// テクスチャセット（必要なら）
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture = GameResources::GetInstance()->GetTexture("noize");
		deferredContext->PSSetShaderResources(0, 1, texture.GetAddressOf());

		// サンプラーセット（もし使うなら）
		ID3D11SamplerState* sampler = states->LinearWrap();
		deferredContext->PSSetSamplers(0, 1, &sampler);

		// ブレンドステート設定
		deferredContext->OMSetBlendState(states->AlphaBlend(), nullptr, 0xFFFFFFFF);
		}

	// モデルを描画
	model->Draw(deferredContext, *states, world, view, proj, false);

	// シェーダー解除（もし必要なら）
	if (m_currentEffect != ENEMY_EFFECT::NONE)
	{
		if (m_currentEffect == ENEMY_EFFECT::DAMAGE)
		{
			m_damageShader->EndSharder(deferredContext);
		}
		else if (m_currentEffect == ENEMY_EFFECT::DEAD)
		{
			m_deadShader->EndSharder(deferredContext);
		}
	}
}

// ---------------------------------------------
/// <summary>
/// シェーダーの作成
/// </summary>
// ---------------------------------------------
void EnemyEffect::CreateShader()
{
	// デバイスの取得
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	// 死亡時のシェーダーの作成
	m_deadShader = std::make_unique<CustomShader>
		(
			device,			// デバイス
			DEAD_VS_PATH,	// 頂点シェーダー
			DEAD_PS_PATH,	// ピクセルシェーダー
			nullptr,		// ジオメトリシェーダー(使用無し)
			INPUT_LAYOUT	// 入力レイアウト
		);

	// ダメージ時のシェーダーの作成
	m_damageShader = std::make_unique<CustomShader>
		(
			device,			// デバイス
			DAMAGE_VS_PATH,	// 頂点シェーダー
			DAMAGE_PS_PATH,	// ピクセルシェーダー
			nullptr,		// ジオメトリシェーダー(使用無し)
			INPUT_LAYOUT	// 入力レイアウト
		);
}

// ---------------------------------------------
/// <summary>
/// 定数バッファの作成
/// </summary>
// ---------------------------------------------
void EnemyEffect::CreateConstBuffer()
{
	// デバイスの取得
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	// 定数バッファの作成
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(ConstBuffer);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	DX::ThrowIfFailed(
		device->CreateBuffer(&desc, nullptr, m_buffer.GetAddressOf())
	);
}

// ---------------------------------------------
/// <summary>
/// 定数バッファの更新処理
/// </summary>
/// <param name="cb">更新対象の定数バッファ</param>
// ---------------------------------------------
void EnemyEffect::UpdateConstBuffer(ConstBuffer* cb)
{
	// 死亡時の時間計測
	if (m_currentEffect == ENEMY_EFFECT::DEAD)
	{
		// 時間の設定（イージングと正規化をしてある状態のもの）
		cb->Time = Easing::easeInCirc(m_totalTime / DEAD_TIME);
	}
	// ダメージ時の時間計測
	else if (m_currentEffect == ENEMY_EFFECT::DAMAGE)
	{
		// 時間の設定（イージングと正規化をしてある状態のもの）
		cb->Time = Easing::easeInCirc(m_totalTime / DAMAGE_TIME);
	}

	cb->Padding = DirectX::SimpleMath::Vector3::Zero;
}

// ---------------------------------------------
/// <summary>
/// エフェクトの設定
/// </summary>
/// <param name="effect">適用するエフェクト</param>
// ---------------------------------------------
void EnemyEffect::SetEffect(ENEMY_EFFECT effect)
{
	// エフェクトの設定
	m_currentEffect = effect;

	if (m_currentEffect == ENEMY_EFFECT::DEAD)
	{
		// 残り時間をリセット
		m_totalTime = DEAD_TIME;
	}
	else if (m_currentEffect == ENEMY_EFFECT::DAMAGE)
	{
		// 残り時間をリセット
		m_totalTime = DAMAGE_TIME;
	}
}