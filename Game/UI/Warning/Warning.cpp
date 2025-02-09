// ------------------------------------------------------------------
// 
// 名前:	Warning.cpp
// 機能:	HPが一定値以下になった時に警告を表示するクラス
// 製作:	池田桜輔
// 
// ------------------------------------------------------------------
// インクルード
#include "pch.h"
#include "Warning.h"
#include "Game/CommonResources.h"
#include "Game/GameResources.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "Game/Data/HPSystem.h"
#include "CommonStates.h"

// ---------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="hp">HPシステム</param>
// ---------------------------------------------------------
Warning::Warning(HPSystem* hp)
	:
	m_hp(hp),
	m_customShader{},
	m_CBuffer{},
	m_states{},
	m_batch{},
	m_texture{},
	m_elapsedTime{},
	m_totalTime{}
{
}

// ---------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------------------------
Warning::~Warning()
{
	Finalize();
}

// ---------------------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// ---------------------------------------------------------
void Warning::Initialize()
{
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

	// テクスチャを取得
	m_texture = GameResources::GetInstance()->GetTexture("warning");

	//	シェーダーにデータを渡すためのコンスタントバッファ生成
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
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------------------
void Warning::Update(float elapsedTime)
{
	if (m_hp->GetHP() > LOW_HP)
		return;

	// 経過時間の加算
	m_totalTime += elapsedTime;
}

// ---------------------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
// ---------------------------------------------------------
void Warning::Render()
{
	using namespace DirectX;
	ID3D11DeviceContext* context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// 頂点情報
	VertexPositionColorTexture vertex[4] =
	{ VertexPositionColorTexture(SimpleMath::Vector3::Zero,SimpleMath::Vector4::One,XMFLOAT2(0.0f, 0.0f)) };

	// バッファの作成
	ConstBuffer cbuff;
	cbuff.matWorld = SimpleMath::Matrix::Identity;
	cbuff.matView = SimpleMath::Matrix::Identity;
	cbuff.matProj = SimpleMath::Matrix::Identity;
	cbuff.diffuse = SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	cbuff.easing = SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	cbuff.time = SimpleMath::Vector4(m_totalTime, 0.0f, 0.0f, 0.0f);

	// コンスタントバッファの設定
	context->UpdateSubresource(m_CBuffer.Get(), 0, nullptr, &cbuff, 0, 0);

	// シェーダーの開始
	m_customShader->BeginSharder(context);

	// シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	// サンプラーステートの設定
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	// ブレンドステートの設定
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	context->RSSetState(m_states->CullNone());

	// テクスチャの設定
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());

	// 描画
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
void Warning::Finalize()
{
}