// ---------------------------------------------------------------------------------------
// 名前:	QuestRenderer.cpp
// 内容:	チュートリアル中の操作案内の描画を行うクラス
// 作成:	池田桜輔
// ---------------------------------------------------------------------------------------
// インクルード
#include "pch.h"
#include "QuestRenderer.h"
#include "../QuestManager.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/UI/UIAnchor.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "CommonStates.h"
#include "Game/GameResources.h"

// -------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="manager">クエストマネージャー</param>
// -------------------------------------------------------
QuestRenderer::QuestRenderer(QuestManager* manager)
	: 
	m_questManager(manager),
	m_position(INIT_POSITION_X, INIT_POSITION_Y, 0),
	m_angle{},
	m_scale{INIT_SIZE},
	m_alpha{},
	m_dissolve{},
	m_currentTime{},
	m_canChanegQuest(false),
	m_clearFlag(false)
{
}

// -------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// -------------------------------------------------------
QuestRenderer::~QuestRenderer()
{
	Finalize();
}

// -------------------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
/// <param name="texture">テクスチャ</param>
// -------------------------------------------------------
void QuestRenderer::Initialize(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{
	// テクスチャを設定
	m_texture = texture;
	// ディゾルブテクスチャの設定
	m_dissolveTexture = GameResources::GetInstance()->GetTexture("noize");

	// デバイスの取得
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// シェーダーの生成
	CreateShader();
	// コンスタントバッファの作成
	ConstantBuffer();

	// コモンステートの生成
	m_states = std::make_unique<DirectX::CommonStates>(device);
	// プリミティブバッチの生成
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(context);

	// スライドアクションの追加
	AddSlideAction();
}

// -------------------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// -------------------------------------------------------
void QuestRenderer::Update(float elapsedTime)
{
	// 経過時間の保存
	m_elapsedTime = elapsedTime;

	// スライドの更新処理
	m_currentAction();
}

// -------------------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
// -------------------------------------------------------
void QuestRenderer::Draw()
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
	context->PSSetShaderResources(1, 1, m_dissolveTexture.GetAddressOf());

	//	板ポリゴンを描画
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	m_batch->End();

	// シェーダー終了
	m_shader->EndSharder(context);
}

// -------------------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
// -------------------------------------------------------
void QuestRenderer::Finalize()
{
	// テクスチャの解放
	m_texture.Reset();
	// シェーダーの解放
	m_shader.reset();
	// コンスタントバッファの解放
	m_CBuffer.Reset();
	// ステートの解放
	m_states.reset();
	// プリミティブバッチの解放
	m_batch.reset();
}

// -------------------------------------------------------
/// <summary>
/// カスタムシェーダーの生成
/// </summary>
// -------------------------------------------------------
void QuestRenderer::CreateShader()
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

// -------------------------------------------------------
/// <summary>
/// 定数バッファの作成
/// </summary>
// -------------------------------------------------------
void QuestRenderer::ConstantBuffer()
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

// -------------------------------------------------------
/// <summary>
/// 定数バッファの更新処理
/// </summary>
// -------------------------------------------------------
void QuestRenderer::UpdateConstantBuffer()
{
	// コンテキストの取得
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// 定数バッファの設定
	CBuffer cbuff;
	cbuff.windowSize = DirectX::SimpleMath::Vector4(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, 0.0f);
	cbuff.alpha = m_alpha;
	cbuff.dissolve = m_dissolve;
	cbuff.padding = DirectX::SimpleMath::Vector2(0.0f, 0.0f);

	// 受け渡し用バッファの内容更新
	context->UpdateSubresource(m_CBuffer.Get(), 0, nullptr, &cbuff, 0, 0);

	// シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);
}

// -------------------------------------------------------
/// <summary>
/// レンダーステートの設定処理
/// </summary>
// -------------------------------------------------------
void QuestRenderer::SetRenderState()
{
	// コンテキストの取得
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	//	画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	ID3D11BlendState* blendstate = m_states->NonPremultiplied(); 	//	半透明描画指定
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);		// 透明処理
	context->OMSetDepthStencilState(m_states->DepthNone(), 0);		// 深度バッファに書き込み参照しない
	context->RSSetState(m_states->CullClockwise());					// カリングは左回り
}

// -------------------------------------------------------
/// <summary>
/// スライドアクションの登録
/// </summary>
// -------------------------------------------------------
void QuestRenderer::AddSlideAction()
{
	// アクションの追加
	m_slideActions.push_back([this]() { return NoSlideTexture(); }); // 待機
	m_slideActions.push_back([this]() { return SlideOutTexture(); }); // スライドアウト
	m_slideActions.push_back([this]() { return SlideCoolTime();	 }); // クールタイム
	m_slideActions.push_back([this]() { return SlideInTexture(); }); // スライドイン

	// 現在のアクションを設定
	m_currentAction = m_slideActions[SLIDE_ACTION::SLIDE_IN];

	// 経過時間を-から始めることで遅延を設定
	m_currentTime = INITIAL_TIME;
}

// -------------------------------------------------------	
/// <summary>
/// 画像をスライドさせない
/// </summary>
// -------------------------------------------------------
void QuestRenderer::NoSlideTexture()
{
	// マネージャーがクエストの変更を許可しているか
	if (m_clearFlag)
	{
		// クエストの変更
		m_currentAction = m_slideActions[SLIDE_ACTION::SLIDE_OUT];
	}
}

// -------------------------------------------------------
/// <summary>
/// テクスチャをスライドアウトさせる
/// </summary>
// -------------------------------------------------------
void QuestRenderer::SlideOutTexture()
{
	// 経過時間の更新
	m_currentTime += m_elapsedTime;
	// 正規化した時間の取得
	float easing = Easing::easeInOutQuad(m_currentTime / FIER_TIME);
	// ディゾルブの設定
	m_dissolve = easing;

	// アニメーション時間を経過した場合、終了
	if (m_currentTime > FIER_TIME)
	{
		// スライドアウト終了
		m_currentTime = 0.0f;
		// スライドアクションの変更
		m_currentAction = m_slideActions[SLIDE_ACTION::COOL_TIME];
		// クリアフラグを折る
		m_clearFlag = false;
		// クエストを１つ進める
		m_questManager->ChangeNextQuest();
	}
}

// -------------------------------------------------------
/// <summary>
/// スライドのクールタイム
/// </summary>
// -------------------------------------------------------
void QuestRenderer::SlideCoolTime()
{
	// クールタイムの更新
	m_currentTime += m_elapsedTime;

	// クールタイムの終了
	if (m_currentTime > ANIMATION_WAIT)
	{
		m_currentTime = 0.0f;
		m_currentAction = m_slideActions[SLIDE_ACTION::SLIDE_IN];
	}
}

// -------------------------------------------------------
/// <summary>
/// テクスチャをスライドインさせる
/// </summary>
// -------------------------------------------------------
void QuestRenderer::SlideInTexture()
{
	// ディゾルブの初期化
	m_dissolve = 0.0f;
	// 経過時間の更新
	m_currentTime += m_elapsedTime;
	// 正規化した時間の取得
	float t = m_currentTime / ANIMATION_TIME;
	// alpha値の設定
	m_alpha = t;
	// スライド処理
	m_position.x = WINDOW_WIDTH - WIDTH * Easing::easeOutExpo(t);
	// アニメーション時間を経過した場合、終了
	if (m_currentTime > ANIMATION_TIME)
	{
		// スライドイン終了
		m_currentTime = 0.0f;
		m_currentAction = m_slideActions[SLIDE_ACTION::WAIT];
	}
}