#pragma once

#include "pch.h"
#include "DeviceResources.h"



class Fade
{
public:

	//	�f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
	};

	//	�֐�
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	static const wchar_t* TEXTURE_PATH;

	static constexpr float FADE_TIME = 3.0f;

	Fade();	// �R���X�g���N�^
	~Fade();	// �f�X�g���N�^

	void Initialize();	// ����������
	void Update();		// �X�V����
	void Render();		// �`�揈��

private:
	void LoadTexture(const wchar_t* path);		// �e�N�X�`���̓ǂݍ���
	void CreateShader();						// �V�F�[�_�[�̐�������
	void CountTimer();

	float m_totalTime;

	DX::DeviceResources*						m_pDR;		// �f�o�C�X���\�[�V�[�Y
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_CBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_input_Layout;

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m__batch;
	std::unique_ptr<DirectX::CommonStates> m_states;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>	m_geometryShader;

};