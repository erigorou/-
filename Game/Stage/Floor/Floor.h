// -----------------------------------
// ＊プリミティブによる地面の描画
// -----------------------------------

#pragma once
#include "pch.h"

class Floor
{
public:
	// -----------------------------
	// 固定値（定数）
	// -----------------------------
	// 円の半径
	static constexpr float RADIUS = 200.0f;
	// 円を分割する数
	static constexpr int SEGMENTS = 16;
	// 正規化用変数
	static constexpr float NORMALIZE = 0.5f;

	// -----------------------------
	// メンバ関数（公開）
	// -----------------------------
public:
	// コンストラクタ
	Floor();
	// デストラクタ
	~Floor();
	// 円の頂点を生成
	void GenerateCircleVertices(
		DirectX::VertexPositionTexture* vertices, 
		float radius, 
		int segments
	);
	// 描画処理
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj); 
	// 終了処理
	void Finalize();

private:
	// -----------------------------
	// メンバ変数（非公開）
	// -----------------------------
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// 共通ステート（描画状態）
	std::unique_ptr<DirectX::CommonStates> m_states;
	// エフェクト（シェーダ）
	std::unique_ptr<DirectX::AlphaTestEffect> m_BatchEffect;
	// プリミティブバッチ（描画用）
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_Batch;
	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
};
