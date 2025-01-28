/*
	@file	QuestSelectScene.h
	@brief	�^�C�g���V�[���N���X
*/
#pragma once
#include "pch.h"
#include "IScene.h"


// �O���錾
class CommonResources;
class Camera;
class Floor;
class Sea;
class TitleEnemy;
class SkySphere;
class Particle;
class CustomShader;

class IconUI;
class QuestSelectSceneUIManager;
class UIAnchor;

namespace mylib
{
	class DebugCamera;
	class GridFloor;
}


class QuestSelectScene final : public IScene
{
public:
	static constexpr float DELAY = 1.5f;		// �^�C�g����ʂ̒x������
	static constexpr float ANIM_END = 1.5f;		// �^�C�g����ʂ̃A�j���[�V�����I������

	static constexpr float STAGE_SELECT_DELAY = 2.5f;	// �X�e�[�W�Z���N�g��ʂ̒x������
	static constexpr float STAGE_SELECT_END	  = 1.0f;	// �X�e�[�W�Z���N�g��ʂ̃A�j���[�V�����I������

	static constexpr float TITLE_LOGO_CENTER_Y = -165.0f;	// �^�C�g���̒��S���W ��bottom�

	static constexpr float WINDOW_WIDTH = 1280.0f;	// �E�B���h�E�̕�
	static constexpr float WINDOW_HEIGHT = 720.0f;	// �E�B���h�E�̍���

	static constexpr int MAX_STAGE_INDEX = 1;
	static constexpr int MIN_STAGE_INDEX = 0;

	// ���̓��C�A�E�g
	std::vector<D3D11_INPUT_ELEMENT_DESC> InputElements =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};


	// �萔�o�b�t�@
	struct CBuffer
	{
		DirectX::SimpleMath::Vector4	windowSize;	// �E�B���h�E�T�C�Y
		float							alpha;		// �����x
		DirectX::SimpleMath::Vector3	padding;	// ��
	};

public:
	void CleateSpamDust(DirectX::SimpleMath::Vector3 pos);

public:

	// �R���X�g���N�^
	QuestSelectScene();
	// �f�X�g���N�^
	~QuestSelectScene() override;

	// ����������
	void Initialize() override;
	// �X�V����
	void Update(float elapsedTime)override;
	// �`�揈��
	void Render() override;
	// �I������
	void Finalize() override;

	// �X�e�[�g�ύX����
	SceneID GetNextSceneID() const;

private:

	// �V�F�[�_�[�̐���
	void CreateShader(const wchar_t* VS, const wchar_t* PS, const wchar_t* GS);

	// �R���X�^���g�o�b�t�@�̍쐬
	void CreateConstantBuffer();

	// �I�u�W�F�N�g�X�V����
	void UpdateObject(const float elapsedTime);

	//// �萔�o�b�t�@�̍X�V����
	//void UpdateConstantBuffer();

	// �`��ݒ�
	void SetRenderState();
	// �摜�̃��[�h����
	void LoadTextures();
	// �摜�̒��S�F�傫�����擾
	void CalculateTextureCenters();

	void CalculateTextureCenter(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture, DirectX::SimpleMath::Vector2& texCenter);


	// �I�u�W�F�N�g�𐶐�
	void CreateObjects();
	// �摜�̕`�揈��
	void DrawTexture();

	// �X�e�[�W�̕`��
	void DrawStageSelect();

	// �X�e�[�W�I��
	void SelectStage(DirectX::Keyboard::KeyboardStateTracker* keyboard);

	// ���ʃ��\�[�X
	CommonResources* m_commonResources;

	std::unique_ptr<DirectX::SpriteBatch>	m_spriteBatch;	// �X�v���C�g�o�b�`
	DirectX::SimpleMath::Matrix				m_projection;	// �v���W�F�N�V�����s��

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texture;		// �e�N�X�`���P
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texture2;		// �e�N�X�`���Q

	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textureList; // �e�N�X�`�����X�g

	DirectX::SimpleMath::Vector2 m_texCenter;	// �e�N�X�`���̒��S���W

	DirectX::SimpleMath::Vector2 m_texCenter1;
	DirectX::SimpleMath::Vector2 m_texCenter2;
	DirectX::SimpleMath::Vector2 m_texCenter3;
	DirectX::SimpleMath::Vector2 m_texCenter4;

	std::vector<std::unique_ptr<CustomShader>> m_shaderList;	// �V�F�[�_�[���X�g
	std::vector<std::unique_ptr<Microsoft::WRL::ComPtr<ID3D10ShaderResourceView>>> m_texList;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;

	// �V�[���`�F���W�t���O
	bool m_isChangeScene;

	// �o�ߎ��Ԃ��擾
	float m_totalSeconds;

	// �I�u�W�F�N�g�B
	std::unique_ptr<Camera>		m_camera;		//�@�J����
	std::unique_ptr<Floor>		m_floor;		// ��
	std::unique_ptr<Sea>		m_sea;			// �C
	std::unique_ptr<TitleEnemy> m_enemy;		// �G
	std::unique_ptr<SkySphere>	m_skySphere;	// �X�J�C�X�t�B�A
	std::unique_ptr<Particle> m_particle;		// �p�[�e�B�N��

	int m_selectIndex;	// �I�𒆂̃X�e�[�W�ԍ�

	// �J������h�炷��
	float m_shakePower;

	std::unique_ptr<QuestSelectSceneUIManager> m_uiManager;
};