#include "pch.h"

#include "../Header/Warning.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "Game/Player/PlayerHP.h"
#include "CommonStates.h"


// 固定値
const wchar_t* Warning::TEXTURE_PATH = L"Resources/Textures/Warning.png";
const wchar_t* Warning::VS_PATH = L"Resources/Shaders/Warning/WarningVS.cso";
const wchar_t* Warning::PS_PATH = L"Resources/Shaders/Warning/WarningPS.cso";
const wchar_t* Warning::GS_PATH = L"Resources/Shaders/Warning/WarningGS.cso";


// コンストラクタ
Warning::Warning(PlayerHP* hp)
	: m_hp(hp)
	, m_pDR(nullptr)
	, m_customShader(nullptr)
	, m_CBuffer(nullptr)
	, m_states(nullptr)
	, m_batch(nullptr)
	, m_texture()
	, m_elapsedTime(0.0f)
	, m_totalTime(0.0f)
{
	// デバイスリソースの取得
	m_pDR = CommonResources::GetInstance()->GetDeviceResources();
}


// デストラクタ
Warning::~Warning()
{
}


// 初期化処理
void Warning::Initialize()
{
	ID3D11Device* device = m_pDR->GetD3DDevice();

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
			m_pDR->GetD3DDeviceContext()
		);

	// コモンステートの生成
	m_states = std::make_unique<DirectX::CommonStates>(device);

	// テクスチャの読み込み
	CustomShader::LoadTexture(device, TEXTURE_PATH, m_texture);

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


// 更新処理
void Warning::Update(float elapsedTime)
{
	if (m_hp->GetHP() > LOW_HP) 
		return;

	// 経過時間の加算
	m_totalTime += elapsedTime;


	if (m_elapsedTime >= 1.0f)
	{
	}
}


// 描画処理
void Warning::Render()
{
	using namespace DirectX;
	ID3D11DeviceContext* context = m_pDR->GetD3DDeviceContext();

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
	cbuff.matWorld	= SimpleMath::Matrix::Identity;
	cbuff.matView	= SimpleMath::Matrix::Identity;
	cbuff.matProj	= SimpleMath::Matrix::Identity;
	cbuff.diffuse	= SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	cbuff.easing	= SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	cbuff.time		= SimpleMath::Vector4(m_totalTime, 0.0f, 0.0f, 0.0f);
	
	// コンスタントバッファの設定
	context->UpdateSubresource(m_CBuffer.Get(), 0, nullptr, &cbuff, 0, 0);

	// シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);
	
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	ID3D11BlendState* blendstate = m_states->NonPremultiplied();
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	context->RSSetState(m_states->CullNone());

	m_customShader->BeginSharder(context);

	for (int i = 0; i < m_texture.size(); i++)
	{
		context->PSSetShaderResources(i, 1, m_texture[i].GetAddressOf());
	}

	context->IASetInputLayout(m_customShader->GetInputLayout());

	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 4);
	m_batch->End();

	m_customShader->EndSharder(context);
}


// 終了処理
void Warning::Finalize()
{
}
