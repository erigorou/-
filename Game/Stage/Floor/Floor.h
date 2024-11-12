// -----------------------------------
// ���v���~�e�B�u�ɂ��n�ʂ̕`��
// -----------------------------------

#pragma once
#include "pch.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"

class Floor
{

// �Œ�l //
public:
	static constexpr float	RADIUS = 175.0f;
	static constexpr int	SEGMENTS = 16;

	static const wchar_t* VS_PATH;
	static const wchar_t* GS_PATH;
	static const wchar_t* PS_PATH;



	// �f�[�^�󂯓n���p�R���X�g�o�b�t�@�i���M���j
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
		DirectX::SimpleMath::Vector4	time;
	};


// ���J�֐� //
public:
	// �R���X�g���N�^
	Floor();
	// �f�X�g���N�^
	~Floor();
	void GenerateCircleVertices(
		DirectX::VertexPositionTexture* vertices, float radius, int segments);
	// �`�揈��
	void Render(
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj);
	// �I������
	void Finalize();

// �����֐� //
private:
	void DrawFloor();
	void DrawSea();


// �����ϐ� //
private:
	std::vector<D3D11_INPUT_ELEMENT_DESC> InputElements =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// ���ʃX�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	//	�G�t�F�N�g
	std::unique_ptr<DirectX::AlphaTestEffect> m_BatchEffect;
	//	�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_Batch;

	//  �e�N�X�`��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;

	// ���`��p //

	// �J�X�^���V�F�[�_�[
	std::unique_ptr<CustomShader>				m_voronoiShader;
	// �R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_CBuffer;


	// �C�`��p //
	float m_totalTime;

};