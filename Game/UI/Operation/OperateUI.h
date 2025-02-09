#pragma once

#include "pch.h"
#include "Interface/IObserver.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"

class OperateUI : public IObserver
{
	// �Œ�l
public:
	static constexpr wchar_t VS_PATH[] = L"Resources/cso/OperationVS.cso";
	static constexpr wchar_t PS_PATH[] = L"Resources/cso/OperationPS.cso";
	static constexpr wchar_t GS_PATH[] = L"Resources/cso/OperationGS.cso";

	static constexpr float DOWN_FADE_TIME = 0.01667f * 10.0f;

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
	OperateUI(std::string_view key);
	~OperateUI();

	void Initialize();
	void Render();
	void Finalize();

	void OnKeyPressed(const DirectX::Keyboard::Keys& key) override;
	void OnKeyDown(const DirectX::Keyboard::Keys& key) override;

	void HitAction(InterSectData data) override;
	DirectX::SimpleMath::Vector3 GetPosition() override;

	// �v���C�x�[�g�֐�
private:

	// �v���C�x�[�g�ϐ�
private:

	std::unique_ptr<CustomShader>			m_customShader;		// �V�F�[�_�[
	DX::DeviceResources* m_pDR;									// �f�o�C�X���\�[�X
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;			// �R���X�^���g�o�b�t�@
	std::unique_ptr<DirectX::CommonStates>	m_states;			// �X�e�[�g

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;	// �v���~�e�B�u�o�b�`

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;	// �e�N�X�`��

	float m_elapsedTime;	// �t���[������
	float m_totalTime;		// �o�ߎ���

	bool m_downKey;

	const wchar_t* m_texturePath;
};