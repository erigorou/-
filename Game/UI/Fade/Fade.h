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

	//	�֐�
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	
	static const wchar_t* TEXTURE_PATH;
	static const wchar_t* VS_PATH;
	static const wchar_t* GS_PATH;
	static const wchar_t* PS_PATH;

	static const wchar_t* STENCIL_TEX_PATH;
	static const wchar_t* MASK_VS_PATH;
	static const wchar_t* MASK_GS_PATH;
	static const wchar_t* MASK_PS_PATH;


	static constexpr float CHANGE_SCENE_TIME = 2.2f;
	static constexpr float FADE_TIME = 2.0f;


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

	float CalcrateFadeValue(float t);
	float CalcurateFadeIn(float t);
	float CalcurateFadeOut(float t);

	void DrawStencilImage();					// �^�����摜�̕`�揈��

	SceneManager* m_scene;		// �V�[���}�l�[�W��

	float		m_elapsedTime;	// 1F�̎���
	float		m_totalTime;	// �o�ߎ���
	bool		m_isFade;		// �t�F�[�h�����ǂ���
	FadeType	m_fadeType;		// �t�F�[�h�̏��

	float		m_easingValue;	// �C�[�W���O�l

	DX::DeviceResources*						m_pDR;		// �f�o�C�X���\�[�V�[�Y
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_CBuffer;	// �R���X�^���g�o�b�t�@

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	std::unique_ptr<DirectX::CommonStates> m_states;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;

	// �J�X�^���V�F�[�_�[
	std::unique_ptr<CustomShader> m_customShader;

	std::unique_ptr<CustomShader> m_maskShader;

	// �^�����p�摜
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_stencilImage;

	// �^�����摜�݂̂̕`�挋�ʂ����摜
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_capture;
	// �^�����摜�݂̂̕`�挋�ʂ����摜�̃r���[
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_captureRTV;
	// �^�����摜�݂̂̕`�挋�ʂ����摜�̃V�F�[�_�[���\�[�X�r���[
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_captureSRV;

};

#endif // !FADE_DEFINED