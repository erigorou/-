//--------------------------------------------------------------------------------------
//
// 名前:	IconUI.cpp
// 内容:	アイコンUIのクラス
//			画像の描画などを簡単に行える
// 作成:	池田桜輔
//
//-------------------------------------------------------------------------------------
// インクルード
#include "pch.h"
#include "IconUI.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "DeviceResources.h"


// -------------------------------------------------------
// 定数
// -------------------------------------------------------

// インプットレイアウト
const std::vector<D3D11_INPUT_ELEMENT_DESC> IconUI::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

// -------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// -------------------------------------------------------
IconUI::IconUI()
	:
	m_windowHeight{}
	, m_windowWidth{}
	, m_textureHeight{}
	, m_textureWidth{}
	, m_texture(nullptr)
	, m_scale(DirectX::SimpleMath::Vector2::One)
	, m_position(DirectX::SimpleMath::Vector2::Zero)
	, m_anchor(ANCHOR::TOP_LEFT)
{
}

// -------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// -------------------------------------------------------
IconUI::~IconUI()
{
}

// -------------------------------------------------------
/// <summary>
/// 生成処理
/// </summary>
/// <param name="position">座標</param>
/// <param name="scale">拡大倍率</param>
/// <param name="anchor">アンカーポイント</param>
// -------------------------------------------------------
void IconUI::Create(
	DirectX::SimpleMath::Vector2 position
	, DirectX::SimpleMath::Vector2 scale
	, ANCHOR anchor)
{
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	//	座標、拡大倍率、アンカーポイントの設定
	m_position = position;
	m_baseScale = m_scale = scale;
	m_anchor = anchor;

	//	シェーダーの作成
	CreateShader();

	//	プリミティブバッチの作成
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(context);
	m_states = std::make_unique<DirectX::CommonStates>(device);
}


// -------------------------------------------------------
/// <summary>
/// Shader作成部分だけ分離した関数
/// </summary>
// -------------------------------------------------------
void IconUI::CreateShader()
{
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	シェーダーの生成
	m_customShader = std::make_unique<CustomShader>(
		device,
		VS_PATH,
		PS_PATH,
		GS_PATH,
		INPUT_LAYOUT
	);

	//	シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}

// -------------------------------------------------------
/// <summary>
/// 描画関数
/// </summary>
// -------------------------------------------------------
void IconUI::Render()
{
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// 頂点情報
	// Position.xy	:拡縮用スケール
	// Position.z	:アンカータイプ(0～8)の整数で指定
	// Color.xy　	:アンカー座標(ピクセル指定:1280 ×720)
	// Color.zw		:画像サイズ
	// Tex.xy		:ウィンドウサイズ（バッファも同じ。こちらは未使用）
	DirectX::VertexPositionColorTexture vertex[1] = {
		DirectX::VertexPositionColorTexture(
			DirectX::SimpleMath::Vector3(m_scale.x, m_scale.y, static_cast<float>(m_anchor)),
			DirectX::SimpleMath::Vector4(m_position.x, m_position.y,static_cast<float>(m_textureWidth),static_cast<float>(m_textureHeight)),
			DirectX::SimpleMath::Vector2(static_cast<float>(m_windowWidth),
			static_cast<float>(m_windowHeight)))
	};
	//	ただし上記の設定値には、WorldやViewなどの3D空間から変換するための計算を一切しないため、
	//	スクリーン座標として描画される

	//	シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.windowSize = DirectX::SimpleMath::Vector4(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight), 1, 1);

	//	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//	シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//	画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//	半透明描画指定
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	//	透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	//	深度バッファに書き込み参照する
	context->OMSetDepthStencilState(m_states->DepthNone(), 0);

	//	カリングは左周り
	context->RSSetState(m_states->CullNone());

	// シェーダーの登録
	m_customShader->BeginSharder(context);

	//	ピクセルシェーダにテクスチャを登録する。
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());

	//	板ポリゴンを描画
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	m_batch->End();

	//	シェーダーの終了処理
	m_customShader->EndSharder(context);
}