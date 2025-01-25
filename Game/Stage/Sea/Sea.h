// ----==========----------==========-------
// 海を描画する為のクラス
// ----==========----------==========-------

#ifndef SEA_H
#define SEA_H

#pragma once
#include "pch.h"

class CustomShader;

class Sea
{
// 固定値 =======
public:
	// データの受け渡し用構造体
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;
		DirectX::SimpleMath::Matrix matView;
		DirectX::SimpleMath::Matrix matProj;
		DirectX::SimpleMath::Vector4 diffuse;
		DirectX::SimpleMath::Vector4 easing;
		DirectX::SimpleMath::Vector4 time;
	};

	// 入力要素配列
	std::vector<D3D11_INPUT_ELEMENT_DESC> InputElements =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// シェーダーのファイルパス
	static constexpr wchar_t VS_PATH[] = L"Resources/cso/VoronoiVS.cso";
	static constexpr wchar_t PS_PATH[] = L"Resources/cso/VoronoiPS.cso";
	static constexpr wchar_t GS_PATH[] = L"Resources/cso/VoronoiGS.cso";


// 公開関数 =============
public:
	Sea();
	~Sea();
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

// 内部関数 ============
private:

	// 生成
	void Create();


// 内部変数 ============
private:
	// コンストバッファー
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;
	// 入力
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	// ステート
	std::unique_ptr<DirectX::CommonStates>	m_states;

	// シェーダー
	std::unique_ptr<CustomShader> m_customShader;

	float m_totalTime;
};


#endif // !SEA_H