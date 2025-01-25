#pragma once
#ifndef FADE_DEFINED
#define FADE_DEFINED

#include "pch.h"
#include "DeviceResources.h"
#include "Game/CommonResources.h"

class SceneManager;
class CustomShader;

class Fade
{
public:
	// �t�F�[�h�̏��
	enum class FadeType : UINT
	{
		FADE_IN,	// ���@���@��
		FADE_OUT,	// �Á@���@��
		END_DELAY,	// �t�F�[�h�I��
		FADE_NONE,	// �Ȃ�
	};


	//	�f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
		DirectX::SimpleMath::Vector4	time;
	};

	// �C���v�b�g���C�A�E�g
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	// �t�F�[�h�Ɏg�p����p�X
	static constexpr wchar_t VS_PATH[] = L"Resources/cso/FadeVS.cso";
	static constexpr wchar_t GS_PATH[] = L"Resources/cso/FadeGS.cso";
	static constexpr wchar_t PS_PATH[] = L"Resources/cso/FadePS.cso";

	// �}�X�N�Ɏg�p����p�X
	static constexpr wchar_t MASK_VS_PATH[] = L"Resources/cso/MaskVS.cso";
	static constexpr wchar_t MASK_GS_PATH[] = L"Resources/cso/MaskGS.cso";
	static constexpr wchar_t MASK_PS_PATH[] = L"Resources/cso/MaskPS.cso";


	// �t�F�[�h�̐ݒ�
	static constexpr float CHANGE_SCENE_EDLAY = 1.0f;
	static constexpr float FADE_TIME = 1.0f;

	static constexpr float FADE_FIRST_SIZE = 1.25f;
	static constexpr float FADE_MAX_SIZE = 6.0f;
	static constexpr float FADE_THRESHOLD = 0.7f;


	void StartFadeIn();
	void StartFadeOut();
	void FadeStop();

	Fade(SceneManager* scene);			// �R���X�g���N�^
	~Fade();							// �f�X�g���N�^
	void Initialize();					// ����������
	void Update(float elapsedTime);		// �X�V����
	void Render();						// �`�揈��


private:
	void CreateShader();						// �V�F�[�_�[�̐�������
	void CountTimer();							// �^�C�}�[�̃J�E���g����

	void FadeIn();
	void FadeOut();

	void FadeEnd();

	float CalcrateFadeValue(float t);

	void DrawStencilImage();					// �^�����摜�̕`�揈��

	SceneManager* m_scene;		// �V�[���}�l�[�W��

	float		m_elapsedTime;	// 1F�̎���
	float		m_totalTime;	// �o�ߎ���
	float		m_delayTime;		// �t�F�[�h����
	bool		m_isFade;		// �t�F�[�h�����ǂ���
	bool		m_endFade;		// �t�F�[�h�I�����ǂ���
	FadeType	m_fadeType;		// �t�F�[�h�̏��

	float		m_easingValue;	// �C�[�W���O�l


	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_CBuffer;	// �R���X�^���g�o�b�t�@

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	std::unique_ptr<DirectX::CommonStates> m_states;

	// �J�X�^���V�F�[�_�[
	std::unique_ptr<CustomShader> m_customShader;
	// �}�X�N�V�F�[�_�[
	std::unique_ptr<CustomShader> m_maskShader;

	// ���[�h��ʂƂ��Ďg�p����e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// �}�X�N�p�̃e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stencilImage;

	// �^�����摜�݂̂̕`�挋�ʂ����摜
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_capture;
	// �^�����摜�݂̂̕`�挋�ʂ����摜�̃r���[
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_captureRTV;
	// �^�����摜�݂̂̕`�挋�ʂ����摜�̃V�F�[�_�[���\�[�X�r���[
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_captureSRV;

};

#endif // !FADE_DEFINED