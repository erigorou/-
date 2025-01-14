// -----------------------------------
// ���v���~�e�B�u�ɂ��n�ʂ̕`��
// -----------------------------------

#pragma once
#include "pch.h"

class Floor
{
public:
	// �Œ�l
	static const wchar_t* TEXTURE_PATH;

	static constexpr float	RADIUS = 175.0f;
	static constexpr int	SEGMENTS = 16;


	// �R���X�g���N�^
	Floor();
	// �f�X�g���N�^
	~Floor();
	void GenerateCircleVertices(DirectX::VertexPositionTexture* vertices, float radius, int segments);
	// �`�揈��
	void Render(
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj);
	// �I������
	void Finalize();

private:
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// ���ʃX�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	//	�G�t�F�N�g
	std::unique_ptr<DirectX::AlphaTestEffect> m_BatchEffect;
	//	�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_Batch;

	//  �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
};