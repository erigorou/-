// --------------------------------------------------
// ���O:	ResultScene.h
// ���e:	���U���g�V�[����`�悷��N���X
// �쐬:	�r�c����
// --------------------------------------------------
#pragma once
#include "IScene.h"
#include "Game/Data/GameData.h"

// �O���錾
class CommonResources;
class WinResult;
class LoseResult;

class ResultScene final : public IScene
{
	// �Œ�l
public:

	// -----------------------------
	// �����o�֐�(���J)
	// -----------------------------

public:
	// �R���X�g���N�^
	ResultScene();
	// �f�X�g���N�^
	~ResultScene() override;
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

	// -----------------------------
	// �����o�֐�(����J)
	// -----------------------------

private:
	// �e�N�X�`���̍쐬
	void CreateTextures();
	// �I�u�W�F�N�g�̍쐬
	void CreateObjects();

	// -----------------------------
	// �����o�ϐ�
	// -----------------------------

private:
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// �e�N�X�`���̒��S���W
	DirectX::SimpleMath::Vector2 m_texCenter;

	// �������U���g
	std::unique_ptr<WinResult>	m_winResult;
	// �s�k���U���g
	std::unique_ptr<LoseResult> m_loseResult;
	// ���݂̃X�e�[�g
	IScene* m_currentState;
	// �V�[���J�ڃt���O
	bool m_isChangeScene;
	// �Q�[���f�[�^
	GameData* m_gameData;
	// �퓬����
	GameData::BATTLE_RESULT m_result;
};
