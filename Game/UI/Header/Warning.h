#pragma once
#ifndef WARNING_DEFINED
#define WARNING_DEFINED

#include "pch.h"
#include "DeviceResources.h"
#include "Libraries/Microsoft/DebugDraw.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"

class HPSystem;

class Warning
{
// �Œ�l
private:
	static const wchar_t* TEXTURE_PATH; 
	static const wchar_t* VS_PATH;
	static const wchar_t* PS_PATH;
	static const wchar_t* GS_PATH;

	static constexpr int LOW_HP = 2;


	std::vector<D3D11_INPUT_ELEMENT_DESC> InputElements =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};


	static constexpr float FADE_TIME = 1.2f;


	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld	;
		DirectX::SimpleMath::Matrix		matView		;
		DirectX::SimpleMath::Matrix		matProj		;
		DirectX::SimpleMath::Vector4	diffuse		;
		DirectX::SimpleMath::Vector4	easing		;
		DirectX::SimpleMath::Vector4	time		;
	};


// �p�u���b�N�֐�
public:
	Warning(HPSystem* hp);
	~Warning();


	void Initialize();
	void Update(float elapsedTime);
	void Render();
	void Finalize();

// �v���C�x�[�g�֐�
private:



// �v���C�x�[�g�ϐ�
private:
	
	HPSystem* m_hp;

	std::unique_ptr<CustomShader>			m_customShader;		// �V�F�[�_�[
	DX::DeviceResources*					m_pDR;				// �f�o�C�X���\�[�X
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;			// �R���X�^���g�o�b�t�@
	std::unique_ptr<DirectX::CommonStates>	m_states;			// �X�e�[�g

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;	// �v���~�e�B�u�o�b�`
	
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;	// �e�N�X�`��

	float m_elapsedTime;	// �t���[������
	float m_totalTime;		// �o�ߎ���
};


#endif // WARNING_DEFINED

