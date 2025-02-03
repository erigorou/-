#pragma once
#ifndef OPERATION_DEFINED
#define OPERATION_DEFINED

#include "pch.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "OperateUI.h"
#include "../UI.h"

class Operation
{
	// �Œ�l
private:
	static const wchar_t* TEXT_PATH;

	static const wchar_t* X_PATH;
	static const wchar_t* SHIFT_PATH;
	static const wchar_t* UP_PATH;
	static const wchar_t* DOWN_PATH;
	static const wchar_t* LEFT_PATH;
	static const wchar_t* RIGHT_PATH;

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
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	diffuse;
		DirectX::SimpleMath::Vector4	easing;
		DirectX::SimpleMath::Vector4	time;
	};

	// �p�u���b�N�֐�
public:
	Operation();
	~Operation();

	void Initialize();
	void Update(float elapsedTime);
	void Render();
	void Finalize();

	// �v���C�x�[�g�֐�
private:

	// �v���C�x�[�g�ϐ�
private:

	std::unique_ptr<CustomShader>			m_customShader;		// �V�F�[�_�[
	DX::DeviceResources* m_pDR;				// �f�o�C�X���\�[�X
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;			// �R���X�^���g�o�b�t�@
	std::unique_ptr<DirectX::CommonStates>	m_states;			// �X�e�[�g

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;	// �v���~�e�B�u�o�b�`

	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;	// �e�N�X�`��

	float m_elapsedTime;	// �t���[������
	float m_totalTime;		// �o�ߎ���

	std::vector<std::unique_ptr<OperateUI>> m_operateUIs;	// ����UI
	std::unique_ptr<UI> m_textUI;							// UI
};

#endif // Operation_DEFINED
