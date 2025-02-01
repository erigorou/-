//--------------------------------------------------------------------------------------
//	File: Particle.h
//
//	パーティクルクラス
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Fade.h"
#include "Libraries/MyLib/BinaryFile.h"
#include "Game/Scene/SceneManager.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/GameResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Game/Data/GameData.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "Libraries/MyLib/Math.h"


// 固定値 **
const std::vector<D3D11_INPUT_ELEMENT_DESC> Fade::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


/// <summary>
/// コンストラクタ
/// </summary>
Fade::Fade(SceneManager* scene)
	: 
	m_scene(scene),
	m_easingValue{},
	m_elapsedTime{},
	m_totalTime{},
	m_delayTime{},
	m_isFade(false),
	m_endFade(false),
	m_fadeType(FadeType::FADE_NONE)
{
}


/// <summary>
/// デストラクタ
/// </summary>
Fade::~Fade()
{
}


/// <summary>
/// 初期化関数
/// </summary>
void Fade::Initialize()
{
	// リソースの取得
	auto resources = CommonResources::GetInstance();
	// デバイスの取得
	auto device = resources->GetDeviceResources()->GetD3DDevice();
	// コンテキストの取得
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();

	// フェードテクスチャの取得
	m_texture = GameResources::GetInstance()->GetTexture("fade");
	// 型抜きテクスチャの取得
	m_stencilImage = GameResources::GetInstance()->GetTexture("mask");
	// シェーダーの生成
	CreateShader();
	// プリミティブバッチの生成
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(context);
	// コモンステートの生成
	m_states = std::make_unique<DirectX::CommonStates>(device);
}


/// <summary>
/// シェーダー作成部分用関数
/// </summary>
void Fade::CreateShader()
{
	// デバイスの取得
	ID3D11Device* device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	// カスタムシェーダーの初期化
	m_customShader = std::make_unique<CustomShader>
		(
			device,
			VS_PATH,
			PS_PATH,
			GS_PATH,
			INPUT_LAYOUT
		);

	// マスク用カスタムシェーダーの初期化
	m_maskShader = std::make_unique<CustomShader>
		(
			device,
			MASK_VS_PATH,
			MASK_PS_PATH,
			MASK_GS_PATH,
			INPUT_LAYOUT
		);

	// シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage			= D3D11_USAGE_DEFAULT;
	bd.ByteWidth		= sizeof(ConstBuffer);
	bd.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags	= 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}

/// <summary>
/// フェードインの開始処理
/// </summary>
/// <param name="type"></param>
void Fade::StartFadeIn()
{
	m_isFade = true;
	m_totalTime = 0;
	m_fadeType = FadeType::FADE_IN;
}


void Fade::StartFadeOut()
{
	m_isFade = true;
	m_totalTime = FADE_TIME;
	m_fadeType = FadeType::FADE_OUT;
}

void Fade::FadeStop()
{
	m_isFade = false;
	m_fadeType = FadeType::FADE_NONE;
}



void Fade::Update(float elapsedTime)
{
	m_elapsedTime = elapsedTime;

	// フェード中でない場合は何もしない
	if ( ! m_isFade) return;

	// タイマーのカウント
	CountTimer();
}


void Fade::CountTimer()
{
	switch (m_fadeType)
	{
	case FadeType::FADE_IN:		FadeIn();	break;
	case FadeType::FADE_OUT:	FadeOut();	break;
	case FadeType::END_DELAY:	FadeEnd();	break;
	
	default:								break;
	}
}



/// <summary>
/// フェードインの処理
/// </summary>
void Fade::FadeIn()
{
	// 時間を計算する( 0 → 1 )
	m_totalTime = std::min(m_totalTime + m_elapsedTime, FADE_TIME);

	if (m_totalTime >= FADE_TIME)
	{
		// フェード中ではない
		m_isFade = false;
	}
}


/// <summary>
/// フェードアウト処理
/// </summary>
void Fade::FadeOut()
{
	// 時間を計算する( 1 → 0 )
	m_totalTime = std::max(m_totalTime - m_elapsedTime, 0.0f);

	if (m_totalTime <= 0.0f)
	{
		// フェードが終了したことを通知
		m_endFade = true;
		// フェード終了処理
		m_fadeType = FadeType::END_DELAY;
		// ディレイ開始
		m_delayTime = CHANGE_SCENE_EDLAY;
	}
}


void Fade::FadeEnd()
{
	// 時間を減らす
	m_delayTime -= m_elapsedTime;

	// 遅延込みで0秒になったら
	if (m_delayTime <= 0.0f)
	{
		// フェード中ではない
		m_isFade = false;
		// フェード終了
		m_endFade = false;
		// シーンに変更フラグを立てる
		m_scene->SetCanChangeScene();
		// フェード無し
		m_fadeType = FadeType::FADE_NONE;
	}
}


// 切り抜き用の画像を取得する
void Fade::DrawStencilImage()
{
	// デバイスリソースの取得
	auto deviceResources = CommonResources::GetInstance()->GetDeviceResources();

	ID3D11DeviceContext* context = deviceResources->GetD3DDeviceContext();

	//	描画についての設定を行う
	D3D11_TEXTURE2D_DESC texDesc;
	deviceResources->GetRenderTarget()->GetDesc(&texDesc);
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	deviceResources->GetD3DDevice()->CreateTexture2D(&texDesc, NULL, m_capture.ReleaseAndGetAddressOf());

	//	レンダーターゲットビューの設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	//	レンダーターゲットビューの生成
	deviceResources->GetD3DDevice()->CreateRenderTargetView(m_capture.Get(), &rtvDesc, &m_captureRTV);

	//	シェーダリソースビューの設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

	//	レンダーターゲットビューと深度ビューを取得（後で元に戻すため）
	ID3D11RenderTargetView* defaultRTV = deviceResources->GetRenderTargetView();
	ID3D11DepthStencilView* pDSV = deviceResources->GetDepthStencilView();

	// 背景色の取得(透明)
	float backColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	// レンダーターゲットと深度ステンシルの設定
	context->OMSetRenderTargets(1, m_captureRTV.GetAddressOf(), pDSV);
	context->ClearRenderTargetView(m_captureRTV.Get(), backColor);
	context->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// ---オブジェクトの描画--- ★

	m_maskShader->BeginSharder(context);

	// フェードの時間を 0 ~ 1で正規化させる
	float t = m_totalTime / FADE_TIME;

	// フェードのサイズを計算
	float size = CalcrateFadeValue(t);

	DirectX::SimpleMath::Matrix world;
	world = DirectX::SimpleMath::Matrix::CreateScale(size, size, 1.0f);

	// シェーダーに渡す情報の媒体を作成する
	ConstBuffer cbuff;

	// 渡すデータを登録する
	cbuff.matView = DirectX::SimpleMath::Matrix::Identity;						// スクリーン座標で描画するため不要
	cbuff.matProj = DirectX::SimpleMath::Matrix::Identity;						// スクリーン座標で描画するため不要	
	cbuff.matWorld = world.Transpose();
	cbuff.Diffuse = DirectX::SimpleMath::Vector4::One;							// テクスチャの色
	cbuff.time = DirectX::SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f);				// フェードの時間（第一要素のみ使用）

	//	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	// シェーダーにバッファを渡す
	ID3D11Buffer* ccb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, ccb);
	context->GSSetConstantBuffers(0, 1, ccb);
	context->PSSetConstantBuffers(0, 1, ccb);

	//	画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//	描画するオブジェクトの情報を設定
	DirectX::VertexPositionColorTexture vertex[4] =
	{
		DirectX::VertexPositionColorTexture(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f),DirectX::SimpleMath::Vector4::One, DirectX::SimpleMath::Vector2(0.0f, 0.0f)),
	};

	//	ピクセルシェーダにテクスチャを登録する。
	context->PSSetShaderResources(0, 1, m_stencilImage.GetAddressOf());

	// (実際には表示しないが) 描画を行う
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 4);
	m_batch->End();


	//	描画した画面をcaptureSRVに保存する
	deviceResources->GetD3DDevice()->CreateShaderResourceView
	(
		m_capture.Get(), &srvDesc, m_captureSRV.ReleaseAndGetAddressOf()
	);

	//	シェーダの解除
	m_maskShader->EndSharder(context);

	//	保持しておいたデフォルト設定に戻して、画面描画が正常に出来るようにしておく
	context->OMSetRenderTargets(1, &defaultRTV, pDSV);
	// 使用した物を解放
	context->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 0.0f, 0);
}



void Fade::Render()
{
	// デバイスリソースの取得
	auto deviceResources = CommonResources::GetInstance()->GetDeviceResources();

	using namespace DirectX;

	// 型抜き画像の描画
	DrawStencilImage();

	float t = 0.0f;
	t = std::max(0.0f, 1.0f - (m_totalTime / FADE_TIME));
	t = std::max(0.0001f, Easing::easeInCubic(t));
	GameData::GetInstance()->SetFadeValue(1 - t);

	ID3D11DeviceContext1* context = deviceResources->GetD3DDeviceContext();

	VertexPositionColorTexture vertex[4] =
	{
		VertexPositionColorTexture(SimpleMath::Vector3(0.0f, 0.0f, 0.0f),SimpleMath::Vector4::One, SimpleMath::Vector2(0.0f, 0.0f)),
	};

	// シェーダーに渡す情報の媒体を作成する
	ConstBuffer cbuff;

	// 渡すデータを登録する
	cbuff.matView = SimpleMath::Matrix::Identity;						// スクリーン座標で描画するため不要
	cbuff.matProj = SimpleMath::Matrix::Identity;						// スクリーン座標で描画するため不要	
	cbuff.matWorld = SimpleMath::Matrix::Identity;						// スクリーン座標で描画するため不要	
	cbuff.Diffuse = SimpleMath::Vector4::One;							// テクスチャの色
	cbuff.time = SimpleMath::Vector4(t, 0.0f, 0.0f, 0.0f);				// フェードの時間（第一要素のみ使用）

	//	シェーダの開始
	m_customShader->BeginSharder(context);

	//	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	// シェーダーにバッファを渡す
	ID3D11Buffer* ccb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, ccb);
	context->GSSetConstantBuffers(0, 1, ccb);
	context->PSSetConstantBuffers(0, 1, ccb);

	//	画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	//	半透明描画指定
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();
	//	透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	//	深度バッファに書き込み参照する
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	//	カリングは正面のみ行う
	context->RSSetState(m_states->CullCounterClockwise());

	//	ピクセルシェーダにテクスチャを登録する。
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
	// マスク画像をピクセルシェーダに登録する
	context->PSSetShaderResources(1, 1, m_captureSRV.GetAddressOf());

	// 板ポリゴンで描画
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 4);
	m_batch->End();

	//	シェーダーの解除
	m_customShader->EndSharder(context);
}

// ----------------------------------
// フェードの値を計算する
// ----------------------------------
float Fade::CalcrateFadeValue(float t)
{
	if		(FadeType::FADE_IN == m_fadeType)	return Easing::easeBetweenIn(t, FADE_THRESHOLD, FADE_FIRST_SIZE, FADE_MAX_SIZE);
	else if (FadeType::FADE_OUT == m_fadeType)	return Easing::easeBetweenOut(t, FADE_THRESHOLD, FADE_FIRST_SIZE, FADE_MAX_SIZE);

	else return 0;
}