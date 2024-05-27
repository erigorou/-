#pragma once

class SkyeBox
{
	// �X�J�C�{�b�N�X�̃e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_skyboxTexture;
	//
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_skyboxSampler;
	// �W�I���g���b�N�v���~�e�B�u
	std::unique_ptr<DirectX::GeometricPrimitive> m_skyboxMesh;
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_skyboxEffect;
	//
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_skyboxRasterState;
	//
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_skyboxInputLayout;
};