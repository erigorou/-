
#include "pch.h"
#include "CustomShader.h"
#include "../BinaryFile.h"


// �R���X�g���N�^
CustomShader::CustomShader
(
	ID3D11Device*									device,
	const wchar_t*									vertexPath,
	const wchar_t*									pixelPath,
	const wchar_t*									geometryPath,
	const std::vector<D3D11_INPUT_ELEMENT_DESC>&	inputElements
)
{
	//	���_�V�F�[�_�[�̓ǂݍ���
	if (vertexPath != nullptr) { LoadVertexShader(device, vertexPath, inputElements); }
	//	�s�N�Z���V�F�[�_�[�̓ǂݍ���
	if (pixelPath != nullptr) { LoadPixelShader(device, pixelPath); }
	//	�W�I���g���V�F�[�_�[�̓ǂݍ���
	if (geometryPath != nullptr) { LoadGeometryShader(device, geometryPath); }
}


// �e�N�X�`���̓ǂݍ���
void CustomShader::LoadTexture(ID3D11Device* device, const wchar_t* path, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& textureArray)
{
	// �e�N�X�`����ۑ�����ϐ�
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;

	// �e�N�X�`���̓ǂݍ���
	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(
			device,
			path,
			nullptr,
			texture.ReleaseAndGetAddressOf()
		)
	);

	// �z��Ɋi�[
	textureArray.push_back(texture);
}


// �V�F�[�_�̊J�n
void CustomShader::BeginSharder(ID3D11DeviceContext* context)
{
	//	�쐬����Ă���V�F�[�_�[�̐ݒ�
	if (m_vertexShader		.Get()	!= nullptr) { context->VSSetShader(m_vertexShader.Get(), nullptr, 0); }
	if (m_pixelShader		.Get()	!= nullptr) { context->PSSetShader(m_pixelShader.Get(), nullptr, 0); }
	if (m_geometryShader	.Get()	!= nullptr) { context->GSSetShader(m_geometryShader.Get(), nullptr, 0); }

	//	�쐬����Ă���Γ��̓��C�A�E�g�̓o�^
	if (m_inputLayout.Get() != nullptr) 
	{ context->IASetInputLayout(m_inputLayout.Get()); }
}


// �V�F�[�_�̏I��
void CustomShader::EndSharder(ID3D11DeviceContext* context)
{
	//	�V�F�[�_�̓o�^������
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
}


// ���_�V�F�[�_�[�̓ǂݍ���
void CustomShader::LoadVertexShader(ID3D11Device* device, const wchar_t* filePath, const InputElements& inputElements)
{
	//	�o�C�i���t�@�C���̓ǂݍ���
	BinaryFile VSData = BinaryFile::LoadFile(filePath);

	//	���_�V�F�[�_�̍쐬
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), nullptr, m_vertexShader.GetAddressOf())))
		MessageBox(0, L"���_�V�F�[�_�̐����Ɏ��s���܂���.", NULL, MB_OK);

	//	���̓��C�A�E�g���쐬���Ȃ��Ă���
	if (inputElements.size() <= 0) { return; }

	//	���̓��C�A�E�g�̍쐬
	device->CreateInputLayout(
		&inputElements.begin()[0],
		(UINT)inputElements.size(),
		VSData.GetData(),
		VSData.GetSize(),
		m_inputLayout.GetAddressOf()
	);
}



// �s�N�Z���V�F�[�_�[�̓ǂݍ���
void CustomShader::LoadPixelShader(ID3D11Device* device, const wchar_t* filePath)
{
	//	�o�C�i���t�@�C���̓ǂݍ���
	BinaryFile PSData = BinaryFile::LoadFile(filePath);

	//	�s�N�Z���V�F�[�_�̍쐬
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), nullptr, m_pixelShader.GetAddressOf())))
		MessageBox(0, L"�s�N�Z���V�F�[�_�̐����Ɏ��s���܂���.", NULL, MB_OK);
}



// �W�I���g���V�F�[�_�[�̓ǂݍ���
void CustomShader::LoadGeometryShader(ID3D11Device* device, const wchar_t* filePath)
{
	//	�o�C�i���t�@�C���̓ǂݍ���
	BinaryFile GSData = BinaryFile::LoadFile(filePath);

	//	�W�I���g���V�F�[�_�̍쐬
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), nullptr, m_geometryShader.GetAddressOf())))
		MessageBox(0, L"�W�I���g���V�F�[�_�̐����Ɏ��s���܂���.", NULL, MB_OK);
}
