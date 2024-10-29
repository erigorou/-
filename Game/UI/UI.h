#pragma once

#include "pch.h"
#include "DeviceResources.h"
#include "Libraries/Microsoft/DebugDraw.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"

class UI
{
	// 固定値
private:
	static const wchar_t* VS_PATH;
	static const wchar_t* PS_PATH;
	static const wchar_t* GS_PATH;


	std::vector<D3D11_INPUT_ELEMENT_DESC> InputElements =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};


	static constexpr float FADE_TIME = 1.2f;


	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	diffuse;
		DirectX::SimpleMath::Vector4	easing;
		DirectX::SimpleMath::Vector4	time;
	};


	// パブリック関数
public:
	UI(const wchar_t* texturePath);
	~UI();


	void Initialize();
	void Render();
	void Finalize();

	// プライベート関数
private:



	// プライベート変数
private:

	std::unique_ptr<CustomShader>			m_customShader;		// シェーダー
	DX::DeviceResources*					m_pDR;				// デバイスリソース
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;			// コンスタントバッファ
	std::unique_ptr<DirectX::CommonStates>	m_states;			// ステート

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>	m_batch;	// プリミティブバッチ
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>					m_texture;	// テクスチャ

	const wchar_t* m_texturePath;
};