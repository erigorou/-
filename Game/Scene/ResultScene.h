/*
	@file	ResultScene.h
	@brief	���U���g�V�[���N���X
*/
#pragma once
#include "IScene.h"

#include "../Data/GameData.h"

// �O���錾
class CommonResources;
class GameData;
class WinResult;
class LoseResult;

namespace mylib
{
	class DebugCamera;
}

class ResultScene final : public IScene
{
	// �Œ�l
public:

	// ���ʊ֐�
public:
	ResultScene();
	~ResultScene() override;

	void Initialize() override;
	void Update(float elapsedTime)override;
	void Render() override;
	void Finalize() override;

	SceneID GetNextSceneID() const;

	// �����o�֐�
private:
	void CreateTextures();	// �e�N�X�`���̍쐬
	void CreateObjects();	// �I�u�W�F�N�g�̍쐬

	// �����o�ϐ�
private:
	CommonResources* m_commonResources;								// ���ʃ��\�[�X
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;			// �X�v���C�g�o�b�`
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;		// �e�N�X�`��
	DirectX::SimpleMath::Vector2 m_texCenter;						// �e�N�X�`���̒��S���W

	// ���U���g�V�[���X�e�[�g
	std::unique_ptr<WinResult>	m_winResult;	// �������U���g
	std::unique_ptr<LoseResult> m_loseResult;	// �s�k���U���g

	bool m_isChangeScene;	// �V�[���J�ڃt���O

	GameData* m_gameData;	// �Q�[���f�[�^�i�V�[���œn����j

	GameData::BATTLE_RESULT m_result;	// �퓬����

	IScene* m_currentState;	// ���݂̃X�e�[�g
};
