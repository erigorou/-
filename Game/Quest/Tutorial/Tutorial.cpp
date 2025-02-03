#include "pch.h"
#include "Tutorial.h"
#include "../QuestManager.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/UI/UIAnchor.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "CommonStates.h"

// --------------------
// 固定値
// --------------------

const wchar_t* Tutorial::VS_PATH = L"Resources/cso/Quest_VS.cso";
const wchar_t* Tutorial::PS_PATH = L"Resources/cso/Quest_PS.cso";
const wchar_t* Tutorial::GS_PATH = L"Resources/cso/Quest_GS.cso";

// --------------------
// コンストラクタ
// --------------------
Tutorial::Tutorial(QuestManager* manager)
	: m_questManager(manager)
	, m_position(INIT_POSITION_X, INIT_POSITION_Y, 0)
	, m_angle()
	, m_scale(1.0f, 1.0f)
	, m_alpha(1.0f)
	, m_currentTime()
	, m_timerPlay(false)
	, m_canUseTimer(true)
	, m_alphaFlag(false)
{
}

// --------------------
// デストラクタ
// --------------------
Tutorial::~Tutorial()
{
}

// --------------------
// 初期化処理
// --------------------
void Tutorial::Initialize(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{
	// デバイスの取得
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// シェーダーの生成
	CreateShader();
	// コンスタントバッファの作成
	ConstantBuffer();

	m_texture = texture;

	// コモンステートの生成
	m_states = std::make_unique<DirectX::CommonStates>(device);
	// プリミティブバッチの生成
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(context);
}

// --------------------
// 更新処理
// --------------------
void Tutorial::Update(float elapsedTime)
{
	// タイマーの更新処理
	UpdateTimer(elapsedTime);

	// α値の更新処理
	UpdateAlpha(elapsedTime);
}

// --------------------
// タイマーの更新処理
// --------------------
void Tutorial::UpdateTimer(float elapsedTime)
{
	// タイマーが起動中でないと更新しない
	if (!m_timerPlay) return;

	// 時間を計測
	m_currentTime = std::max((m_currentTime - elapsedTime), 0.0f);

	// タイマーが0以下になったらタイマーを止める
	if (m_currentTime <= 0.0f)	m_timerPlay = false;
}

// --------------------
// α値の更新処理
// --------------------
void Tutorial::UpdateAlpha(float elapsedTime)
{
	// α値を減らすフラグが立っていないと更新しない
	if (!m_alphaFlag) return;

	// 時間を計測
	m_currentTime = std::max((m_currentTime - elapsedTime), 0.0f);

	// 時間を正規化
	float t = m_currentTime / ANIMATION_TIME;

	// α値の更新
	m_alpha = Easing::easeOutQuint(t);
}

// --------------------
// 描画処理
// --------------------
void Tutorial::Draw()
{
	// コンテキストの取得
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// 頂点情報の設定
	DirectX::VertexPositionColorTexture vertex[1] =
	{
		DirectX::VertexPositionColorTexture
		(
			DirectX::SimpleMath::Vector3(m_scale.x, m_scale.y, static_cast<float>(ANCHOR::TOP_LEFT))
		,	DirectX::SimpleMath::Vector4(m_position.x, m_position.y, WIDTH, HEIGHT)
		,	DirectX::SimpleMath::Vector2(WINDOW_WIDTH, WINDOW_HEIGHT)
		)
	};

	// 定数バッファの設定
	UpdateConstantBuffer();

	// 画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	// レンダーステートの設定
	SetRenderState();

	// シェーダー開始
	m_shader->BeginSharder(context);

	// テクスチャの設定
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());

	//	板ポリゴンを描画
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	m_batch->End();

	// シェーダー終了
	m_shader->EndSharder(context);
}

// --------------------
// 終了処理
// --------------------
void Tutorial::Finalize()
{
}

// --------------------
// タイマースタート
// --------------------
void Tutorial::StartTimer()
{
	// タイマーが使用可能でないと処理を行わない
	if (!m_canUseTimer)	return;

	// タイマー起動
	m_timerPlay = true;
	// タイマーの設定
	m_currentTime = ANIMATION_TIME;
	// タイマーを使用不可にする
	m_canUseTimer = false;
}

// --------------------
// α値減らし開始
// --------------------
void Tutorial::StartAlphaMinus()
{
	if (m_alphaFlag) return;

	m_alphaFlag = true;
	m_currentTime = ANIMATION_TIME;
}

// --------------------
// シェーダーの生成
// --------------------
void Tutorial::CreateShader()
{
	// デバイスの取得
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	// シェーダーの生成
	m_shader = std::make_unique<CustomShader>
		(
			device,
			VS_PATH,
			PS_PATH,
			GS_PATH,
			InputElements
		);
}

// --------------------
// コンスタントバッファの作成
// --------------------
void Tutorial::ConstantBuffer()
{
	// デバイスの取得
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	HRESULT hr = device->CreateBuffer(&bd, nullptr, &m_CBuffer);
	if (FAILED(hr)) {
		MessageBox(0, L"コンスタントバッファの生成に失敗しました.", NULL, MB_OK);
	}
}

// --------------------
// 定数バッファの更新処理
// --------------------
void Tutorial::UpdateConstantBuffer()
{
	// コンテキストの取得
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// 定数バッファの設定
	CBuffer cbuff;
	cbuff.windowSize = DirectX::SimpleMath::Vector4(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, 0.0f);
	cbuff.alpha = m_alpha;
	cbuff.padding = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);

	// 受け渡し用バッファの内容更新
	context->UpdateSubresource(m_CBuffer.Get(), 0, nullptr, &cbuff, 0, 0);

	// シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);
}

// --------------------
// レンダーステートの設定
// --------------------
void Tutorial::SetRenderState()
{
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	//	画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	ID3D11BlendState* blendstate = m_states->NonPremultiplied(); 	//	半透明描画指定
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);		// 透明処理
	context->OMSetDepthStencilState(m_states->DepthNone(), 0);		// 深度バッファに書き込み参照しない
	context->RSSetState(m_states->CullClockwise());					// カリングは左回り
}