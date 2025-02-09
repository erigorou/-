/*
	@file	WinResultScene.h
	@brief	���U���g�V�[���N���X
*/
#pragma once
#include "../IScene.h"

// �O���錾
class CommonResources;

class WinResult final : public IScene
{
public:

	// -------------------------
	// �����o�֐�(���J)
	// -------------------------
public:
	// �R���X�g���N�^
	WinResult();
	// �f�X�g���N�^
	~WinResult() override;
	// ����������
	void Initialize() override;
	// �X�V����
	void Update(float elapsedTime)override;
	// �`�揈��	
	void Render() override;
	// �I������
	void Finalize() override;
	// ���̃V�[��ID���擾����
	SceneID GetNextSceneID() const;

	// -------------------------
	// �����o�ϐ�
	// -------------------------
private:
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// �e�N�X�`���̒��S���W
	DirectX::SimpleMath::Vector2 m_texCenter;
	// �V�[���J�ڃt���O
	bool m_isChangeScene;
};
