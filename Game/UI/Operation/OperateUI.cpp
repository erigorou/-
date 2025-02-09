// --------------------------------------------
// 
// 名前:	OperateUI.cpp
// 機能:	操作説明UIのクラス
//			ボタン一つを生成
// 製作:	池田桜輔
// 
// --------------------------------------------
// インクルード
#include "pch.h"
#include "OperateUI.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "CommonStates.h"
#include "Game/GameResources.h"

// ---------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="key">取得するテクスチャのキー</param>
// ---------------------------------------------------------
OperateUI::OperateUI(std::string_view key)
	:
	m_customShader(nullptr),
	m_CBuffer(nullptr),
	m_states(nullptr),
	m_batch(nullptr),
	m_texture(),
	m_elapsedTime(DOWN_FADE_TIME),
	m_totalTime{},
	m_downKey(false)
{
	// テクスチャを取得
	m_texture = GameResources::GetInstance()->GetTexture(static_cast<std::string>(key));
}

// ---------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------------------------
OperateUI::~OperateUI()
{
}

// ---------------------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// ---------------------------------------------------------
void OperateUI::Initialize()
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
/// 描画処理
/// </summary>
// ---------------------------------------------------------
void OperateUI::Render()
{
	using namespace DirectX;
	ID3D11DeviceContext* context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// キーが押されている場合
	if (m_downKey)
	{
		// 経過時間を減らす
		m_totalTime -= m_elapsedTime;

		// 経過時間が0以下になったらキーが押されていない状態にする
		if (m_totalTime <= 0.0f) m_downKey = false;
	}

	// 頂点情報
	VertexPositionColorTexture vertex[4] =
	{
		VertexPositionColorTexture(
		SimpleMath::Vector3::Zero,
		SimpleMath::Vector4::One,
		XMFLOAT2(0.0f, 0.0f)
		)
	};

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

	// ステートの設定
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
void OperateUI::Finalize()
{
	// シェーダーの解放
	m_customShader.reset();
	// プリミティブバッチの解放
	m_batch.reset();
	// ステートの解放
	m_states.reset();
	// テクスチャの解放
	m_texture.Reset();
	// コンスタントバッファの解放
	m_CBuffer.Reset();
}

// ---------------------------------------------------------
/// <summary>
/// 入力処理
/// </summary>
/// <param name="key">押されたキー</param>
// ---------------------------------------------------------
void OperateUI::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}

// ---------------------------------------------------------
/// <summary>
/// 入力処理
/// </summary>
/// <param name="key">押されたキー</param>
// ---------------------------------------------------------
void OperateUI::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	m_downKey = true;
	m_totalTime = 1.0f;

	UNREFERENCED_PARAMETER(key);
}

// ---------------------------------------------------------
/// <summary>
/// 衝突判定処理
/// </summary>
/// <param name="data">衝突情報</param>
// ---------------------------------------------------------
void OperateUI::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}

// ---------------------------------------------------------
/// <summary>
/// 位置取得
/// </summary>
/// <returns>現在の位置</returns>
// ---------------------------------------------------------
DirectX::SimpleMath::Vector3 OperateUI::GetPosition()
{
	return DirectX::SimpleMath::Vector3::Zero;
}