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

	static constexpr float FADE_TIME = 1.2f;

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

	std::unique_ptr<CustomShader> m_customShader;



	// �^�����p�̕ϐ�

	// �^�����p�e�N�X�`��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_maskTexture;

	// �����_�[�e�N�X�`��
	//std::unique_ptr<>
};

#endif // !FADE_DEFINED