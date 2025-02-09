//--------------------------------------------------------------------------------------
// 
// 名前: UserInterface.cpp
// 機能: UIを表示するクラス
// 製作: 池田桜輔
//
//-------------------------------------------------------------------------------------
// インクルード
#include "pch.h"
#include "Game/CommonResources.h"
#include "UserInterface.h"
#include "Libraries/MyLib/BinaryFile.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "Interface/IAction.h"

// ---------------------------------------------------------
// 定数
// ---------------------------------------------------------

// インプットレイアウト
const std::vector<D3D11_INPUT_ELEMENT_DESC> UserInterface::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
// ---------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// ---------------------------------------------------------
UserInterface::UserInterface()
	:
	m_totalTime{},
	m_windowSize{ WINDOW_SIZE },
	m_textureSize{},
	m_texture{},
	m_scale(DirectX::SimpleMath::Vector2::One),
	m_position(),
	m_alpha(1.0f),
	m_anchor(ANCHOR::TOP_LEFT),
	m_action(nullptr)
{
}

// ---------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------------------------
UserInterface::~UserInterface()
{
}

// ---------------------------------------------------------
/// <summary>
/// テクスチャサイズの取得
/// </summary>
/// <param name="srv">テクスチャ</param>
/// <summary>
// ---------------------------------------------------------
void UserInterface::GetTextureSize(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv) {
	// ID3D11Resourceを取得
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
	srv->GetResource(resource.GetAddressOf());

	// ID3D11Texture2Dにキャスト
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	if (FAILED(resource.As(&texture))) {
		// もしテクスチャでない場合のエラーハンドリング
		return;
	}

	// テクスチャのディスクリプタを取得
	D3D11_TEXTURE2D_DESC desc;
	texture->GetDesc(&desc);

	// テクスチャのサイズを取得
	m_textureSize =
		DirectX::SimpleMath::Vector2(
			static_cast<float>(desc.Width),
			static_cast<float>(desc.Height)
		);
}

// ---------------------------------------------------------
/// <summary>
/// 生成関数
/// </summary>
/// <param name="texture">テクスチャ</param>
/// <param name="position">座標</param>
/// <param name="scale">拡大倍率</param>
/// <param name="anchor">アンカーポイント</param>
/// <param name="action">アニメーションアクション</param>
// ---------------------------------------------------------
void UserInterface::Create(
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture,
	DirectX::SimpleMath::Vector2 position,
	DirectX::SimpleMath::Vector2 scale,
	ANCHOR anchor,
	IAction* action)
{
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// 初期座標の設定
	m_position = position;
	// スケールの設定
	m_scale = scale;
	// アンカーの設定
	m_anchor = anchor;
	// アクションの設定
	m_action = action;

	//	シェーダーの作成
	CreateShader();
	// テクスチャを取得
	m_texture = texture;
	// テクスチャの大きさを取得
	GetTextureSize(texture);

	//	プリミティブバッチの作成
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(context);
	// コモンステートの生成
	m_states = std::make_unique<DirectX::CommonStates>(device);
}

// ---------------------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="time">経過時間</param>
// ---------------------------------------------------------
void UserInterface::Update(const float time)
{
	//	経過時間を加算
	m_totalTime += time;

	//	アクションがある場合は実行する
	if (m_action)
	{
		// アクションのパラメータを作成する
		ActionParams param;
		param.position = m_position;
		param.scale = m_scale;
		param.alpha = 1.0f;

		// アクションを実行する
		param = m_action->Execute(param, m_totalTime);

		// アクションの結果を反映する
		m_position = param.position;
		m_scale = param.scale;
		m_alpha = param.alpha;
	}
}

// ---------------------------------------------------------
/// <summary>
/// Shader作成部分だけ分離した関数
/// </summary>
// ---------------------------------------------------------
void UserInterface::CreateShader()
{
	// デバイスの取得
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	// シェーダーの作成
	m_shader = std::make_unique<CustomShader>(
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
	// デバイスにバッファを作成させる
	HRESULT hr = device->CreateBuffer(&bd, nullptr, &m_CBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"コンスタントバッファの生成に失敗しました.", NULL, MB_OK);
	}
}

// ---------------------------------------------------------
/// <summary>
/// 描画関数
/// </summary>
// ---------------------------------------------------------
void UserInterface::Render()
{
	//	デバイスコンテキストの取得
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// 頂点情報
	// Position.xy	:拡縮用スケール
	// Position.z	:アンカータイプ(0～8)の整数で指定
	// Color.xy　	:アンカー座標(ピクセル指定:1280 ×720)
	// Color.zw		:画像サイズ
	// Tex.xy		:ウィンドウサイズ（バッファも同じ。こちらは未使用）
	DirectX::VertexPositionColorTexture vertex[1] =
	{
		DirectX::VertexPositionColorTexture(
			DirectX::SimpleMath::Vector3(m_scale.x, m_scale.y, static_cast<float>(m_anchor)),
			DirectX::SimpleMath::Vector4(m_position.x, m_position.y, m_textureSize.x, m_textureSize.y),
			DirectX::SimpleMath::Vector2(m_windowSize.x, m_windowSize.y))
	};

	//	シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.windowSize = DirectX::SimpleMath::Vector2(m_windowSize.x, m_windowSize.y);
	cbuff.alpha = m_alpha;
	cbuff.padding = 0.0f;

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
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	//	カリングは左周り
	context->RSSetState(m_states->CullNone());
	// シェーダをセットする
	m_shader->BeginSharder(context);
	//	ピクセルシェーダにテクスチャを登録する。
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
	//	板ポリゴンを描画
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	m_batch->End();
	//	シェーダーを取り消す
	m_shader->EndSharder(context);
}