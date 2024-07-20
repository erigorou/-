// -----------------------------------
// ＊プリミティブによる地面の描画
// -----------------------------------

#include "pch.h"
#include "Floor.h"

// -------------------------------------------------------------------^
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="device">デバイスのポインタ</param>
/// // ---------------------------------------------------------------^
Floor::Floor(ID3D11Device1* device)
{
	using namespace DirectX;

	//	エフェクトの作成
	m_BatchEffect = std::make_unique<AlphaTestEffect>(device);
	m_BatchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_BatchEffect->SetReferenceAlpha(255);

	//	入力レイアウト生成
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_BatchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		VertexPositionTexture::InputElements,
		VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf()
	);

	//	共通ステートの作成
	m_states = std::make_unique<CommonStates>(device);

	//	テクスチャのロード
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/floor.png",
		nullptr,
		m_texture.GetAddressOf()
	);
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
/// 床の描画を行う
/// </summary>
/// <param name="context">デバイスコンテキスト</param>
/// <param name="view">ビュー行列</param>
/// <param name="proj">プロジェクション行列</param>
// ---------------------------------------------
void Floor::Render(ID3D11DeviceContext1* context, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	// プリミティブバッチの作成
	m_Batch =
		std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	// 頂点情報（板ポリゴンの頂点）
	VertexPositionTexture vertex[4] =
	{
		// 座標					画像のUV座標：普通は0で設定.超えた場合は繰り返す
		VertexPositionTexture(Vector3(100.0f, 0.0f, 100.0f),	Vector2(-1.0f, -1.0f)),
		VertexPositionTexture(Vector3(-100.0f, 0.0f, 100.0f),	Vector2(1.0f, -1.0f)),
		VertexPositionTexture(Vector3(-100.0f, 0.0f, -100.0f),	Vector2(1.0f,  1.0f)),
		VertexPositionTexture(Vector3(100.0f,0.0f, -100.0f),	Vector2(-1.0f, 1.0f)),
	};

	// テクスチャサンプラーの設定（クランプテクスチャアドレッシングモード）
	ID3D11SamplerState* samplers[1] = { m_states->PointWrap() };
	context->PSSetSamplers(0, 1, samplers);

	// 深度バッファに書き込み参照する
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);

	// カリングは左周り（反時計回り）
	context->RSSetState(m_states->CullNone());

	// 不透明のみ描画する設定
	m_BatchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	m_BatchEffect->SetReferenceAlpha(0);
	m_BatchEffect->SetWorld(SimpleMath::Matrix::Identity);
	m_BatchEffect->SetView(view);
	m_BatchEffect->SetProjection(proj);
	m_BatchEffect->SetTexture(m_texture.Get());
	m_BatchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());

	// 半透明部分を描画
	m_Batch->Begin();
	m_Batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_Batch->End();
}


// -----------------------------
/// <summary>
/// 終了処理
/// </summary>
// -----------------------------
void Floor::Finalize()
{
}