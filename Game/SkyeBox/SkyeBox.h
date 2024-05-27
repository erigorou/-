#pragma once

class SkyeBox
{
	// スカイボックスのテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_skyboxTexture;
	//
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_skyboxSampler;
	// ジオメトリックプリミティブ
	std::unique_ptr<DirectX::GeometricPrimitive> m_skyboxMesh;
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_skyboxEffect;
	//
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_skyboxRasterState;
	//
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_skyboxInputLayout;
};