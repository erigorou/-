/*
	@file	ResultScene.h
	@brief	���U���g�V�[���N���X
*/
#pragma once
#include "IScene.h"

// �O���錾
class CommonResources;
class GameData;

namespace mylib
{
	class DebugCamera;
	class GridFloor;
}


class ResultScene final : public IScene
{
public:
	ResultScene();
	~ResultScene() override;

	void Initialize() override;
	void Update(float elapsedTime)override;
	void Render() override;
	void Finalize() override;

	SceneID GetNextSceneID() const;

private:
	CommonResources* m_commonResources;								// ���ʃ��\�[�X	
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;			// �X�v���C�g�o�b�`
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;		// �e�N�X�`��
	DirectX::SimpleMath::Vector2 m_texCenter;						// �e�N�X�`���̒��S���W


	bool m_isChangeScene;	// �V�[���J�ڃt���O

	GameData* m_gameData;	// �Q�[���f�[�^�i�V�[���œn����j

};
