// -----------------------------------
// ＊プリミティブによる地面の描画
// -----------------------------------

#pragma once
#include "pch.h"

class Floor
{
public:
	// 固定値
	static const wchar_t* TEXTURE_PATH;

	static constexpr float	RADIUS = 175.0f;
	static constexpr int	SEGMENTS = 16;


	// コンストラクタ
	Floor();
	// デストラクタ
	~Floor();
	void GenerateCircleVertices(DirectX::VertexPositionTexture* vertices, float radius, int segments);
	// 描画処理
	void Render(
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj);
	// 終了処理
	void Finalize();

private:
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// 共通ステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	//	エフェクト
	std::unique_ptr<DirectX::AlphaTestEffect> m_BatchEffect;
	//	プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_Batch;

	//  テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
};