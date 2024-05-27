#pragma once

class SkyeBox
{
public:
	// �X�J�C�{�b�N�X�p�̃����o�ϐ�
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_skyboxTexture;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_skyboxSampler;
	std::unique_ptr<DirectX::GeometricPrimitive> m_skyboxMesh;
	std::unique_ptr<DirectX::BasicEffect> m_skyboxEffect;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_skyboxRasterState; // �ǉ�
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_skyboxInputLayout; // �ǉ�
};