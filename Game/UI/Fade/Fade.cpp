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
#include "Libraries/Microsoft/DebugDraw.h"
#include "Libraries/MyLib/DebugString.h"

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


/// <summary>
/// コンストラクタ
/// </summary>
Fade::Fade(SceneManager* scene)
	: m_scene(scene)
	, m_totalTime(FADE_TIME)
	, m_isFade(false)
{
}


/// <summary>
/// デストラクタ
/// </summary>
Fade::~Fade()
{
	// do nothing.
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

	// テクスチャの読み込み
	LoadTexture(TEXTURE_PATH);
	// シェーダーの生成
	CreateShader();
	// プリミティブバッチの生成
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(m_pDR->GetD3DDeviceContext());
	// コモンステートの生成
	m_states = std::make_unique<DirectX::CommonStates>(m_pDR->GetD3DDevice());
}



/// <summary>
/// テクスチャリソース読み込み処理
/// </summary>
/// <param name="path">テクスチャの相対パス</param>
void Fade::LoadTexture(const wchar_t* path)
{
	// テクスチャを保存する変数
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;

	// テクスチャの読み込み
	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(
			m_pDR->GetD3DDevice(),
			path,
			nullptr,
			texture.ReleaseAndGetAddressOf()
		)
	);

	// 配列に格納
	m_texture.push_back(texture);
}


/// <summary>
/// シェーダー作成部分用関数
/// </summary>
void Fade::CreateShader()
{
	// デバイスの取得
	ID3D11Device* device = m_pDR->GetD3DDevice();

	// コンパイルされたシェーダーファイルを読み込む
	BinaryFile VSData = BinaryFile::LoadFile(VS_PATH);
	BinaryFile GSData = BinaryFile::LoadFile(GS_PATH);
	BinaryFile PSData = BinaryFile::LoadFile(PS_PATH);

	// インプットレイアウトの作成
	device->CreateInputLayout(
		&INPUT_LAYOUT[0],
		(UINT)INPUT_LAYOUT.size(),
		VSData.GetData(),
		VSData.GetSize(),
		m_input_Layout.GetAddressOf()
	);

	// 頂点シェーダーの作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), nullptr, m_vertexShader.GetAddressOf())))
		MessageBox(0, L"頂点シェーダーの生成に失敗しました.", NULL, MB_OK);

	// ジオメトリシェーダーの作成
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), nullptr, m_geometryShader.GetAddressOf())))
		MessageBox(0, L"ジオメトリシェーダーの生成に失敗しました.", NULL, MB_OK);

	// ピクセルシェーダーの作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), nullptr, m_pixelShader.GetAddressOf())))
		MessageBox(0, L"ピクセルシェーダーの生成に失敗しました.", NULL, MB_OK);

	//	シェーダーにデータを渡すためのコンスタントバッファ生成
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
	m_totalTime = 0.0f;
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



void Fade::Render()
{
	using namespace DirectX;

	// イージングで使用する為の変数	// 1に近いと明るい
	float t = 0.0f;

	t = std::max(0.0f, 1.0f - (m_totalTime / FADE_TIME));
	t = std::max(0.0001f, Easying::easeInCubic(t));

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
	//	シェーダをセットする
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//	ピクセルシェーダにテクスチャを登録する。
	for (int i = 0; i < m_texture.size(); i++)
		context->PSSetShaderResources(i, 1, m_texture[i].GetAddressOf());

	// インプットレイアウトの登録
	context->IASetInputLayout(m_input_Layout.Get());

	// 板ポリゴンで描画
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 4);
	m_batch->End();

	//	シェーダーの解除
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);


#ifdef _DEBUG
	CommonResources* resources = CommonResources::GetInstance();
	auto debugString = resources->GetDebugString();
	debugString->AddString("t = %f" , t);
#endif // !_DEBUG

}

