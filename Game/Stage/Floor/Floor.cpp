// -----------------------------------
// 名前:	Floor.cpp
// 内容:	円形の地面の描画クラス
// 作成:	池田桜輔
// -----------------------------------

// インクルード
#include "pch.h"
#include "Floor.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Game/GameResources.h"

// --------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="device">デバイスのポインタ</param>
/// --------------------------------------------------
Floor::Floor()
{
	CommonResources* resources = CommonResources::GetInstance();
	auto device = resources->GetDeviceResources()->GetD3DDevice();

	// エフェクトの作成
	m_BatchEffect = std::make_unique<DirectX::AlphaTestEffect>(device);
	m_BatchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_BatchEffect->SetReferenceAlpha(255);

	// 入力レイアウト生成
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_BatchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		DirectX::VertexPositionTexture::InputElements,
		DirectX::VertexPositionTexture::InputElementCount,
		shaderByteCode, static_cast<int>(byteCodeLength), m_inputLayout.GetAddressOf()
	);

	// 共通ステートの作成
	m_states = std::make_unique<DirectX::CommonStates>(device);

	// テクスチャの読み込み
	m_texture = GameResources::GetInstance()->GetTexture("floor");
}

// --------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
/// --------------------------------------------------
Floor::~Floor()
{
}

// --------------------------------------------------
/// <summary>
/// 円の頂点を生成する
/// </summary>
/// <param name="vertices">頂点配列</param>
/// <param name="radius">円の半径</param>
/// <param name="segments">円の分割数</param>
/// --------------------------------------------------
void Floor::GenerateCircleVertices(DirectX::VertexPositionTexture* vertices, float radius, int segments)
{
	// 円の頂点を生成
	for (int i = 0; i < segments; ++i)
	{
		// 1周分の角度を求める
		float angle = (2.0f * DirectX::XM_PI / segments) * i;
		// 頂点座標を設定
		vertices[i].position = DirectX::SimpleMath::Vector3(radius * cosf(angle), 0.0f, radius * sinf(angle));
		// 正規化したテクスチャ座標を取得
		float normalizedSin = sin(angle) * NORMALIZE + NORMALIZE;
		float normalizedCos = cos(angle) * NORMALIZE + NORMALIZE;
		// テクスチャ座標を設定
		vertices[i].textureCoordinate = DirectX::SimpleMath::Vector2(normalizedCos, normalizedSin);
	}
}

// --------------------------------------------------
/// <summary>
/// 床の描画を行う
/// </summary>
/// <param name="context">デバイスコンテキスト</param>
/// <param name="view">ビュー行列</param>
/// <param name="proj">プロジェクション行列</param>
/// --------------------------------------------------
void Floor::Render(
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix proj
)
{
	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	// プリミティブバッチの作成
	m_Batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(context);

	// 頂点情報（円の頂点）
	std::vector<DirectX::VertexPositionTexture> vertices(SEGMENTS);
	GenerateCircleVertices(vertices.data(), RADIUS, SEGMENTS);

	// 深度バッファに書き込み参照する
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);

	// カリングは左周り（反時計回り）
	context->RSSetState(m_states->CullCounterClockwise());

	// 完全不透明のみ描画する設定
	m_BatchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	m_BatchEffect->SetReferenceAlpha(0);
	m_BatchEffect->SetWorld(DirectX::SimpleMath::Matrix::Identity);
	m_BatchEffect->SetView(view);
	m_BatchEffect->SetProjection(proj);
	m_BatchEffect->SetTexture(m_texture.Get());
	m_BatchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());

	// サンプラーステートの設定
	ID3D11SamplerState* sampler[1] = { states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	// 半透明部分を描画
	m_Batch->Begin();

	// 円を描画
	for (int i = 0; i < SEGMENTS; ++i)
	{
		m_Batch->DrawTriangle(vertices[i], vertices[(i + 1) % SEGMENTS], vertices[0]);
	}

	m_Batch->End();
}

// --------------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
/// --------------------------------------------------
void Floor::Finalize()
{
}
