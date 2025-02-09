//--------------------------------------------------------------------------------------
//
// ���O:	IconUI.h
// ���e:	�A�C�R��UI�̃N���X
//			�摜�̕`��Ȃǂ��ȒP�ɍs����
// �쐬:	�r�c����
//
//-------------------------------------------------------------------------------------
// �C���N���[�h
#include "pch.h"
#include "DeviceResources.h"
#include "Game/UI/UIAnchor.h"

// �O���錾
class CustomShader;

// �A�C�R���N���X
class IconUI
{
public:
	//�f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4	windowSize;
	};

	// �V�F�[�_�[�p�X
	static constexpr wchar_t VS_PATH[] = L"Resources/cso/UIVS.cso";
	static constexpr wchar_t PS_PATH[] = L"Resources/cso/UIPS.cso";
	static constexpr wchar_t GS_PATH[] = L"Resources/cso/UIGS.cso";

	// �C���v�b�g���C�A�E�g
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	// -----------------------------
	// �����o�֐�(���J)
	// -----------------------------
public:



	// �R���X�g���N�^
	IconUI();
	// �f�X�g���N�^
	~IconUI();
	// ��������
	void Create(
		DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, ANCHOR anchor
	);
	// �`��֐�
	void Render();

	// -----------------------------
	// �����o�֐�(����J)
	// -----------------------------
private:
	// �V�F�[�_�[�̍쐬
	void CreateShader();

	// -----------------------------
	// �����o�ϐ�
	// -----------------------------
private:
	// �R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	//�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	// �e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// �J�X�^���V�F�[�_�[
	std::unique_ptr<CustomShader> m_customShader;
	// �E�B���h�E�T�C�Y
	int m_windowWidth, m_windowHeight;
	// �e�N�X�`���T�C�Y
	int m_textureWidth, m_textureHeight;
	// �e�N�X�`���̃X�P�[��
	DirectX::SimpleMath::Vector2 m_scale;
	// �e�N�X�`���̃x�[�X�X�P�[��
	DirectX::SimpleMath::Vector2 m_baseScale;
	// �e�N�X�`���̍��W
	DirectX::SimpleMath::Vector2 m_position;
	// �A���J�[�|�C���g
	ANCHOR m_anchor;
};