// -----------------------------------
// ＊プリミティブによる地面の描画
// -----------------------------------

#pragma once
#include "pch.h"

class Floor
{
public:
	// 固定値
	static const float SIZE;


	// コンストラクタ
	Floor(ID3D11Device1* device);
	// デストラクタ
	~Floor();;
	// 描画処理
	void Render(ID3D11DeviceContext1* context, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
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