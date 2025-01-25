// ----==========----------==========-------
// �C��`�悷��ׂ̃N���X
// ----==========----------==========-------

#ifndef SEA_H
#define SEA_H

#pragma once
#include "pch.h"

class CustomShader;

class Sea
{
// �Œ�l =======
public:
	// �f�[�^�̎󂯓n���p�\����
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;
		DirectX::SimpleMath::Matrix matView;
		DirectX::SimpleMath::Matrix matProj;
		DirectX::SimpleMath::Vector4 diffuse;
		DirectX::SimpleMath::Vector4 easing;
		DirectX::SimpleMath::Vector4 time;
	};

	// ���͗v�f�z��
	std::vector<D3D11_INPUT_ELEMENT_DESC> InputElements =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// �V�F�[�_�[�̃t�@�C���p�X
	static constexpr wchar_t VS_PATH[] = L"Resources/cso/VoronoiVS.cso";
	static constexpr wchar_t PS_PATH[] = L"Resources/cso/VoronoiPS.cso";
	static constexpr wchar_t GS_PATH[] = L"Resources/cso/VoronoiGS.cso";


// ���J�֐� =============
public:
	Sea();
	~Sea();
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

// �����֐� ============
private:

	// ����
	void Create();


// �����ϐ� ============
private:
	// �R���X�g�o�b�t�@�[
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;
	// ����
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	// �X�e�[�g
	std::unique_ptr<DirectX::CommonStates>	m_states;

	// �V�F�[�_�[
	std::unique_ptr<CustomShader> m_customShader;

	float m_totalTime;
};


#endif // !SEA_H