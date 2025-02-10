// --------------------------------------------------
// 
// ���O:	ResultScene.h
// ���e:	���U���g�V�[����`�悷��N���X
// �쐬:	�r�c����
// 
// --------------------------------------------------
#pragma once
// �C���N���[�h
#include "IScene.h"
#include "Game/Data/GameData.h"

// �O���錾
class CommonResources;

/// <summary>
/// ���U���g�V�[��
/// </summary>
class ResultScene final : public IScene
{
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

	// -----------------------------
	// �����o�ϐ�
	// -----------------------------

private:
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// �X�N���[���V���b�g
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_captureTexture;

	// �e�N�X�`���̒��S���W
	DirectX::SimpleMath::Vector2 m_texCenter;
	// �X�N�V���e�N�X�`���̒��S���W
	DirectX::SimpleMath::Vector2 m_captureTexCenter;

	// �V�[���J�ڃt���O
	bool m_isChangeScene;
	// �Q�[���f�[�^
	GameData* m_gameData;
	// �퓬����
	GameData::BATTLE_RESULT m_result;
};
