/*
	@file	TitleScene.h
	@brief	�^�C�g���V�[���N���X
*/
#pragma once
#include "IScene.h"


// �O���錾
class CommonResources;
class Camera;
class Floor;
class TitleEnemy;

namespace mylib
{
	class DebugCamera;
	class GridFloor;
}


class TitleScene final : public IScene
{
private:
	DirectX::SimpleMath::Vector2 TITLE_DIRECTION_CENTER = { 0.0f, -75.0f };

public:
	// �R���X�g���N�^
	TitleScene();
	// �f�X�g���N�^
	~TitleScene() override;

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

	void SetShakeCamera();			// �J������h�炷

private:
	// �摜�̃��[�h����
	void LoadTextures();
	// �摜�̒��S�F�傫�����擾
	void CalculateTextureCenter();
	// �I�u�W�F�N�g�𐶐�
	void CreateObjects();
	// �摜�̕`�揈��
	void DrawTexture();

	// ���ʃ��\�[�X
	CommonResources* m_commonResources;

	std::unique_ptr<DirectX::SpriteBatch>	m_spriteBatch;	// �X�v���C�g�o�b�`
	DirectX::SimpleMath::Matrix				m_projection;	// �v���W�F�N�V�����s��

	std::unique_ptr<DirectX::SpriteFont>				m_spriteFont;	// �X�v���C�g�t�H���g
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texture;		// �e�N�X�`���P
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texture2;		// �e�N�X�`���Q

	DirectX::SimpleMath::Vector2 m_texCenter;	// �e�N�X�`���̒��S���W
	DirectX::SimpleMath::Vector2 m_texCenter1;
	DirectX::SimpleMath::Vector2 m_texCenter2;

	// �V�[���`�F���W�t���O
	bool m_isChangeScene;


	// �I�u�W�F�N�g�B
	std::unique_ptr<Camera>		m_camera;	//�@�J����
	std::unique_ptr<Floor>		m_floor;	// ��
	std::unique_ptr<TitleEnemy> m_enemy;	// �G
};

