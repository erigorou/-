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
#include "Libraries/MyLib/DebugString.h"
#include "Game/Data/GameData.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"

///	<summary>
///	インプットレイアウト
///	</summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> Fade::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

const wchar_t* Fade::TEXTURE_PATH	= L"Resources/Textures/Load.png";
const wchar_t* Fade::VS_PATH		= L"Resources/Shaders/Fade/FadeVS.cso";
const wchar_t* Fade::GS_PATH		= L"Resources/Shaders/Fade/FadeGS.cso";
const wchar_t* Fade::PS_PATH		= L"Resources/Shaders/Fade/FadePS.cso";

const wchar_t* Fade::STENCIL_TEX_PATH = L"Resources/Textures/mask.png";
const wchar_t* Fade::MASK_VS_PATH = L"Resources/Shaders/Mask/MaskVS.cso";
const wchar_t* Fade::MASK_GS_PATH = L"Resources/Shaders/Mask/MaskGS.cso";
const wchar_t* Fade::MASK_PS_PATH = L"Resources/Shaders/Mask/MaskPS.cso";


/// <summary>
/// コンストラクタ
/// </summary>
Fade::Fade(SceneManager* scene)
	: m_scene(scene)
	, m_totalTime(0.0f)
	, m_isFade(false)
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
	// コモンリソースの取得（インスタンス）
	CommonResources* commonResources = CommonResources::GetInstance();
	// デバイスリソースの取得
	m_pDR = commonResources->GetDeviceResources();
	auto device = m_pDR->GetD3DDevice();

	m_customShader->LoadTexture(device, TEXTURE_PATH, m_texture);
	// シェーダーの生成
	CreateShader();
	// プリミティブバッチの生成
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(m_pDR->GetD3DDeviceContext());
	// コモンステートの生成
	m_states = std::make_unique<DirectX::CommonStates>(m_pDR->GetD3DDevice());
	// 型抜き画像の読み込み
	m_customShader->LoadTexture(device, STENCIL_TEX_PATH, m_stencilImage);
}


/// <summary>
/// シェーダー作成部分用関数
/// </summary>
void Fade::CreateShader()
{
	// デバイスの取得
	ID3D11Device* device = m_pDR->GetD3DDevice();

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
		m_totalTime = FADE_TIME;
		m_isFade = false;
	}
	else if (m_totalTime >= CHANGE_SCENE_TIME)
	{
		// シーン変更を許可する
		m_scene->SetCanChangeScene();
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
		m_totalTime = 0.0f;
		m_isFade = false;
		// シーン変更を許可する
		m_scene->SetCanChangeScene();
	}
}

void Fade::DrawStencilImage()
{
	// デバイスコンテキストの取得
	ID3D11DeviceContext* context = m_pDR->GetD3DDeviceContext();

	//	描画についての設定を行う
	D3D11_TEXTURE2D_DESC texDesc;
	m_pDR->GetRenderTarget()->GetDesc(&texDesc);
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	m_pDR->GetD3DDevice()->CreateTexture2D(&texDesc, NULL, m_capture.ReleaseAndGetAddressOf());

	//	レンダーターゲットビューの設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	//	レンダーターゲットビューの生成
	m_pDR->GetD3DDevice()->CreateRenderTargetView(m_capture.Get(), &rtvDesc, &m_captureRTV);

	//	シェーダリソースビューの設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

	//	レンダーターゲットビューと深度ビューを取得（後で元に戻すため）
	ID3D11RenderTargetView* defaultRTV = m_pDR->GetRenderTargetView();
	ID3D11DepthStencilView* pDSV = m_pDR->GetDepthStencilView();

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

	//	描画するオブジェクトの情報を設定
	DirectX::VertexPositionColorTexture vertex[4] =
	{
		DirectX::VertexPositionColorTexture(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f),DirectX::SimpleMath::Vector4::One, DirectX::SimpleMath::Vector2(0.0f, 0.0f)),
	};

	//	ピクセルシェーダにテクスチャを登録する。
	for (int i = 0; i < m_texture.size(); i++)
		context->PSSetShaderResources(i, 1, m_stencilImage[i].GetAddressOf());

	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 4);
	m_batch->End();


	//	描画した画面をcaptureSRVに保存する
	HRESULT hr = m_pDR->GetD3DDevice()->CreateShaderResourceView
	(
		m_capture.Get(), &srvDesc, m_captureSRV.ReleaseAndGetAddressOf()
	);

	m_maskShader->EndSharder(context);

	//	保持しておいたデフォルト設定に戻して、画面描画が正常に出来るようにしておく
	backColor[0] = 0.3f;
	backColor[1] = 0.3f;
	backColor[2] = 0.3f;
	backColor[3] = 0.0f;
	context->OMSetRenderTargets(1, &defaultRTV, pDSV);
	context->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 0.0f, 0);
}



void Fade::Render()
{
	using namespace DirectX;

	// 型抜き画像の描画
	DrawStencilImage();

	float t = 0.0f;
	t = std::max(0.0f, 1.0f - (m_totalTime / FADE_TIME));
	t = std::max(0.0001f, Easing::easeInCubic(t));
	GameData::GetInstance()->SetFadeValue(1 - t);

	ID3D11DeviceContext1* context = m_pDR->GetD3DDeviceContext();

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
	for (int i = 0; i < m_texture.size(); i++)
		context->PSSetShaderResources(i, 1, m_texture[i].GetAddressOf());

	context->PSSetShaderResources(m_texture.size(), 1, m_captureSRV.GetAddressOf());


	// 板ポリゴンで描画
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 4);
	m_batch->End();

	//	シェーダーの解除
	m_customShader->EndSharder(context);


#ifdef _DEBUG
#endif // !_DEBUG

}



// ----------------------------------
// フェードの値を計算する
// ----------------------------------
float Fade::CalcrateFadeValue(float t)
{
	if (FadeType::FADE_IN == m_fadeType)
	{
		return Easing::easeInOutExpo(t) * 6;
	}
	else if (FadeType::FADE_OUT == m_fadeType)
	{
		return Easing::easeInOutExpo(t) * 6;
	}
}