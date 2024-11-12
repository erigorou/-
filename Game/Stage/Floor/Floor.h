// -----------------------------------
// ＊プリミティブによる地面の描画
// -----------------------------------

#pragma once
#include "pch.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"

class Floor
{

// 固定値 //
public:
	static constexpr float	RADIUS = 175.0f;
	static constexpr int	SEGMENTS = 16;

	static const wchar_t* VS_PATH;
	static const wchar_t* GS_PATH;
	static const wchar_t* PS_PATH;



	// データ受け渡し用コンストバッファ（送信側）
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
		DirectX::SimpleMath::Vector4	time;
	};


// 公開関数 //
public:
	// コンストラクタ
	Floor();
	// デストラクタ
	~Floor();
	void GenerateCircleVertices(
		DirectX::VertexPositionTexture* vertices, float radius, int segments);
	// 描画処理
	void Render(
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj);
	// 終了処理
	void Finalize();

// 内部関数 //
private:
	void DrawFloor();
	void DrawSea();


// 内部変数 //
private:
	std::vector<D3D11_INPUT_ELEMENT_DESC> InputElements =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// 共通ステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	//	エフェクト
	std::unique_ptr<DirectX::AlphaTestEffect> m_BatchEffect;
	//	プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_Batch;

	//  テクスチャ
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;

	// 床描画用 //

	// カスタムシェーダー
	std::unique_ptr<CustomShader>				m_voronoiShader;
	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_CBuffer;


	// 海描画用 //
	float m_totalTime;

};