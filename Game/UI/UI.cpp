
#include "pch.h"
#include "UI.h"

#include "Game/CommonResources.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "CommonStates.h"

// 固定値
const wchar_t* UI::VS_PATH = L"Resources/Shaders/Operation/OperationVS.cso";
const wchar_t* UI::PS_PATH = L"Resources/Shaders/Operation/OperationPS.cso";
const wchar_t* UI::GS_PATH = L"Resources/Shaders/Operation/OperationGS.cso";


UI::UI(const wchar_t* texturePath)
	: m_texturePath(texturePath)
	, m_pDR(nullptr)
	, m_customShader(nullptr)
	, m_CBuffer(nullptr)
	, m_states(nullptr)
	, m_batch(nullptr)
	, m_texture()
{
	m_pDR = CommonResources::GetInstance()->GetDeviceResources();
}


UI::~UI()
{
}


void UI::Initialize()
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
	CustomShader::LoadTexture(device, m_texturePath, m_texture);

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


void UI::Render()
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
	cbuff.matWorld = SimpleMath::Matrix::Identity;
	cbuff.matView = SimpleMath::Matrix::Identity;
	cbuff.matProj = SimpleMath::Matrix::Identity;
	cbuff.diffuse = SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	cbuff.easing = SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	cbuff.time = SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f);

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

	// テクスチャの設定
	for (int i = 0; i < m_texture.size(); i++)
	{
		context->PSSetShaderResources(i, 1, m_texture[i].GetAddressOf());
	}

	context->IASetInputLayout(m_customShader->GetInputLayout());

	// 描画
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 4);
	m_batch->End();

	m_customShader->EndSharder(context);
}


void UI::Finalize()
{
	m_customShader.reset();
	m_batch.reset();
	m_states.reset();
	m_texture.clear();
	m_CBuffer.Reset();
}