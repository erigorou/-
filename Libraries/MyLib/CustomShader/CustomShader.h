#pragma once
#ifndef CUSTOMSHADER_DEFINED
#define CUSTOMSHADER_DEFINED

#include "pch.h"

class CustomShader
{
public:
	//	���͗v�f�z��
	using InputElements = std::vector<D3D11_INPUT_ELEMENT_DESC>;


	ID3D11InputLayout* GetInputLayout() { return m_inputLayout.Get(); }

	// �R���X�g���N�^
	CustomShader
	(
		ID3D11Device*	device			,
		const wchar_t*	vertexPath		,
		const wchar_t*	pixelPath		,
		const wchar_t*	geometryPath	,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements
	);

	//�@�摜�ǂݎ�菈��
	static void LoadTexture
	(
		ID3D11Device* device,
		const wchar_t* path,
		std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& textureArray
	);


	// �f�X�g���N�^
	~CustomShader() = default;

	// �V�F�[�_��ݒ�
	void BeginSharder(ID3D11DeviceContext* context);
	// �V�F�[�_�[��������
	void EndSharder(ID3D11DeviceContext* context);

private:
	// ���_�V�F�[�_�[�̓ǂݍ���
	void LoadVertexShader(ID3D11Device* device, const wchar_t* filePath, const InputElements& inputElements);
	// �s�N�Z���V�F�[�_�[�̓ǂݍ���
	void LoadPixelShader(ID3D11Device* device, const wchar_t* filePath);
	// �W�I���g���V�F�[�_�[�̓ǂݍ���
	void LoadGeometryShader(ID3D11Device* device, const wchar_t* filePath);


	Microsoft::WRL::ComPtr<ID3D11VertexShader>		m_vertexShader;		// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		m_pixelShader;		// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>	m_geometryShader;	// �W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		m_inputLayout;		// ���̓��C�A�E�g
};


#endif // CUSTOMSHADER_DEFINED