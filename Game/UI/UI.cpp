// ------------------------------------------------------------------
//
// 名前: UI.cpp
// 機能: 画像を簡単に表示するクラス
// 製作: 池田桜輔
//
// ------------------------------------------------------------------
// インクルード
#include "pch.h"
#include "UI.h"
#include "Game/CommonResources.h"
#include "Game/GameResources.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "CommonStates.h"

// ---------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="key">取得するテクスチャのキー</param>
// ---------------------------------------------------------
UI::UI(std::string_view key)
	:
	m_customShader(nullptr)
	, m_CBuffer(nullptr)
	, m_states(nullptr)
	, m_batch(nullptr)
	, m_texture()
{
	// テクスチャを取得
	m_texture = GameResources::GetInstance()->GetTexture(static_cast<std::string>(key));
}

// ---------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------------------------
UI::~UI()
{
}

// ---------------------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// ---------------------------------------------------------
void UI::Initialize()
{
	// デバイス取得
	ID3D11Device* device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	// シェーダーの生成
	m_customShader = std::make_unique<CustomShader>
		(
			device,
			VS_PATH,
			PS_PATH,
			GS_PATH,
			InputElements
		);

	// プリミティブバッチの生成
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>
		(
			CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext()
		);

	// コモンステートの生成
	m_states = std::make_unique<DirectX::CommonStates>(device);

	// コンスタントバッファの生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	HRESULT hr = device->CreateBuffer(&bd, nullptr, &m_CBuffer);
	if (FAILED(hr)) {
		MessageBox(0, L"コンスタントバッファの生成に失敗しました.", NULL, MB_OK);
	}
}

// ---------------------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
// ---------------------------------------------------------
void UI::Render()
{
	using namespace DirectX;
	ID3D11DeviceContext* context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// 頂点情報の作成
	VertexPositionColorTexture vertex[4] =
	{
		VertexPositionColorTexture(
		SimpleMath::Vector3::Zero,
		SimpleMath::Vector4::One,
		XMFLOAT2(0.0f, 0.0f)
		)
	};

	// コンスタントバッファの作成
	ConstBuffer cbuff;
	cbuff.matWorld = SimpleMath::Matrix::Identity;
	cbuff.matView = SimpleMath::Matrix::Identity;
	cbuff.matProj = SimpleMath::Matrix::Identity;
	cbuff.diffuse = SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	cbuff.easing = SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	cbuff.time = SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f);

	// コンスタントバッファの更新
	context->UpdateSubresource(m_CBuffer.Get(), 0, nullptr, &cbuff, 0, 0);

	// シェーダーにコンスタントバッファを設定
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	// サンプラーの設定
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	// ブレンドステートの設定
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	context->RSSetState(m_states->CullNone());

	// シェーダーの開始
	m_customShader->BeginSharder(context);

	// テクスチャの設定
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());

	// 描画開始
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 4);
	m_batch->End();

	// シェーダーの終了
	m_customShader->EndSharder(context);
}

// ---------------------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
// ---------------------------------------------------------
void UI::Finalize()
{
	// リソースの解放
	m_customShader.reset();
	m_batch.reset();
	m_states.reset();
	m_texture.Reset();
	m_CBuffer.Reset();
}
