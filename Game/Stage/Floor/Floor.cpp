// -----------------------------------
// ＊プリミティブによる円形の地面の描画 + 波の描画
// -----------------------------------

#include "pch.h"
#include "Floor.h"
#include "Game/CommonResources.h"
#include "Game/Data/GameData.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"


// ------------
// 固定値
// ------------
const wchar_t* Floor::VS_PATH = L"Resources/Shaders/Voronoi/Voronoi_VS.cso";
const wchar_t* Floor::PS_PATH = L"Resources/Shaders/Voronoi/Voronoi_PS.cso";
const wchar_t* Floor::GS_PATH = L"Resources/Shaders/Voronoi/Voronoi_GS.cso";



// -------------------------------------------------------------------^
/// <summary>
/// コンストラクタ
/// </summary>
/// // ---------------------------------------------------------------^
Floor::Floor()
	: m_totalTime(0.0f)
{
	using namespace DirectX;
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// エフェクトの作成
	m_BatchEffect = std::make_unique<AlphaTestEffect>(device);
	m_BatchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_BatchEffect->SetReferenceAlpha(255);

	// 入力レイアウト生成
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_BatchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		VertexPositionTexture::InputElements,
		VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf()
	);

	// VoronoiShaderの生成
	m_voronoiShader = std::make_unique<CustomShader>
		(
			device,
			VS_PATH,
			PS_PATH,
			GS_PATH,
			InputElements
		);

	// プリミティブバッチの作成
	m_Batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	// 共通ステートの作成
	m_states = std::make_unique<CommonStates>(device);

	// テクスチャの読み込み
	CustomShader::LoadTexture(device, L"Resources/Textures/Floor.png", m_texture);

	//	シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);

	HRESULT hr = device->CreateBuffer(&bd, nullptr, &m_CBuffer);
	if (FAILED(hr)) {
		MessageBox(0, L"コンスタントバッファの生成に失敗しました.", NULL, MB_OK);
	}
}


// ----------------------^
/// <summary>
/// デストラクタ
/// </summary>
// ----------------------^
Floor::~Floor()
{
}


// -------------------------------------------
/// <summary>
/// 円の頂点を生成する
/// </summary>
/// <param name="vertices">頂点配列</param>
/// <param name="radius">円の半径</param>
/// <param name="segments">円の分割数</param>
/// <param name="radius">円の半径</param>
/// -------------------------------------------
void Floor::GenerateCircleVertices(DirectX::VertexPositionTexture* vertices, float radius, int segments)
{
	for (int i = 0; i < segments; ++i)
	{
		float angle = (2.0f * DirectX::XM_PI / segments) * i;
		vertices[i].position = DirectX::SimpleMath::Vector3(radius * cosf(angle), 0.0f, radius * sinf(angle));
		vertices[i].textureCoordinate = DirectX::SimpleMath::Vector2(cosf(angle) * 0.5f + 0.5f, sinf(angle) * 0.5f + 0.5f);
	}
}

// ---------------------------------------------
/// <summary>
/// 床の描画を行う
/// </summary>
/// <param name="context">デバイスコンテキスト</param>
/// <param name="view">ビュー行列</param>
/// <param name="proj">プロジェクション行列</param>
// ---------------------------------------------
void Floor::Render(
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();

	// 深度バッファに書き込み参照する
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	// 裏面の描画をしない
	context->RSSetState(m_states->CullCounterClockwise());

	// 不透明のみ描画する設定
	m_BatchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	m_BatchEffect->SetReferenceAlpha(0);
	m_BatchEffect->SetWorld(SimpleMath::Matrix::Identity);
	m_BatchEffect->SetView(view);
	m_BatchEffect->SetProjection(proj);
	m_BatchEffect->SetTexture(m_texture[0].Get());
	m_BatchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());

	DrawFloor();
	DrawSea();
}


// -----------------------------
/// <summary>
/// 床の描画
/// </summary>
/// -----------------------------
void Floor::DrawFloor()
{
	// 地面描画用の頂点情報
	std::vector<DirectX::VertexPositionTexture> vertices(SEGMENTS);
	GenerateCircleVertices(vertices.data(), RADIUS, SEGMENTS);

	// 半透明部分を描画
	m_Batch->Begin();

	// 地面の円形描画
	for (int i = 0; i < SEGMENTS; ++i)
	{
		m_Batch->DrawTriangle(vertices[i], vertices[(i + 1) % SEGMENTS], vertices[0]);
	}

	m_Batch->End();
}


// -----------------------------
/// <summary>
/// 海の描画
/// </summary>
/// -----------------------------
void Floor::DrawSea()
{
	using namespace DirectX;
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// 頂点情報
	VertexPositionTexture vertex[4] =
	{
		VertexPositionTexture(
		SimpleMath::Vector3::Zero,
		SimpleMath::Vector4::One
		)
	};

	// 経過時間を取得
	m_totalTime += GameData::GetInstance()->GetElapsedTime();

	ConstBuffer cbuff;
	cbuff.matWorld = DirectX::SimpleMath::Matrix::Identity;
	cbuff.matView = DirectX::SimpleMath::Matrix::Identity;
	cbuff.matProj = DirectX::SimpleMath::Matrix::Identity;
	cbuff.Diffuse = DirectX::SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f);
	cbuff.time = DirectX::SimpleMath::Vector4(m_totalTime, 0.0f, 0.0f, 0.0f);

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

	m_voronoiShader->BeginSharder(context);

	context->IASetInputLayout(m_voronoiShader->GetInputLayout());
	
	m_Batch->Begin();
	m_Batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 4);
	m_Batch->End();

	m_voronoiShader->EndSharder(context);
}


// -----------------------------
/// <summary>
/// 終了処理
/// </summary>
// -----------------------------
void Floor::Finalize()
{
}
