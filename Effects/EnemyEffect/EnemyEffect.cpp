// ** ---------------------------------------- **
// <意図>　敵の体のエフェクト実装(死亡)
// <作者>　池田
// <日付>　2024/1/20
// ** ---------------------------------------- **

#include "pch.h"
#include "EnemyEffect.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/CommonResources.h"
#include "Game/GameResources.h"
#include "DeviceResources.h"


// ----------------------------
/// コンストラクタ
// ----------------------------
EnemyEffect::EnemyEffect()
	:
	m_totalTime{},
	m_deadShader{},
	m_buffer{},
	m_currentEffect{ENEMY_EFFECT::NONE}
{
	// シェーダーの生成
	CreateShader();

	// 定数バッファの作成
	CreateConstBuffer();
}



// ----------------------------
/// デストラクタ
// ----------------------------
EnemyEffect::~EnemyEffect()
{
}


// ----------------------------
/// 更新処理
// ----------------------------
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


// ----------------------------
/// 体パーツの描画
// ----------------------------
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


	ID3D11SamplerState* sampler[1] = { };
	context->PSSetSamplers(0, 1, sampler);

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture = GameResources::GetInstance()->GetTexture("noize");

	// モデルの描画
	model->Draw(context, *states, world, view, proj, false, [&]
		{
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


// ----------------------------
/// シェーダーの作成
// ----------------------------
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


// ----------------------------
/// 定数バッファの作成
// ----------------------------
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

// ----------------------------
/// 定数バッファの更新処理
// ----------------------------
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


// ----------------------------
/// どのエフェクトを適用するか
// ----------------------------
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