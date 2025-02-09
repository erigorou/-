/*
	@file	LoseResultScene.h
	@brief	���U���g�V�[���N���X
*/
#pragma once
#include "../IScene.h"

// �O���錾
class CommonResources;

class LoseResult final : public IScene
{
public:
	static const wchar_t* TEXTURE_PATH;	// �e�N�X�`���̃p�X

private:
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;			// �X�v���C�g�o�b�`
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;		// �e�N�X�`��
	DirectX::SimpleMath::Vector2 m_texCenter;						// �e�N�X�`���̒��S���W

	bool m_isChangeScene;	// �V�[���J�ڃt���O

public:
	LoseResult();
	~LoseResult() override;

	void Initialize() override;
	void Update(float elapsedTime)override;
	void Render() override;
	void Finalize() override;

	SceneID GetNextSceneID() const;
};
