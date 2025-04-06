// --------------------------------------------------
// 
// ���O:	SelectScene.h
// ���e:	�Z���N�g�V�[����`�悷��N���X
// �쐬:	�r�c����
// 
// --------------------------------------------------
#pragma once
// �C���N���[�h
#include "IScene.h"
#include "Interface/IObserver.h"
#include "Game/Data/GameData.h"
#include "Game/UI/UIAnchor.h"

// �O���錾
class CommonResources;
class UserInterface;
class IAction;

class SelectScene final : public IScene, public IObserver
{
	// -----------------------------
	// �Œ�l
	// -----------------------------
public:


	// ------------------------------
	// �����o�֐�(���J)
	// ------------------------------
public:
	// �R���X�g���N�^
	SelectScene();
	// �f�X�g���N�^
	~SelectScene() override;
	// ����������
	void Initialize() override;
	// �X�V����
	void Update(float elapsedTime) override;
	// �`�揈��
	void Render() override;
	// �I������
	void Finalize() override;
	// ���̃V�[��ID���擾����
	SceneID GetNextSceneID() const;
	// �O�̃V�[��ID���擾����
	SceneID GetPrevSceneID() const;
	// �L�[���́i�����ꂽ�u�ԁj
	void OnKeyPressed(const DirectX::Keyboard::Keys& key) override;
	// �L�[���́i������Ă���ԁj
	void OnKeyDown(const DirectX::Keyboard::Keys& key) override;

	// ------------------------------
	// �����o�ϐ�(����J)
	// ------------------------------
private:
	// UI�̒ǉ�
	void AddUserInterface(
		const std::string& texName,
		const DirectX::SimpleMath::Vector2& position,
		const DirectX::SimpleMath::Vector2& size,
		const ANCHOR& anchor,
		IAction* action
	);

	// ���[�U�[�C���^�[�t�F�[�X�̒ǉ�
	void CreateUI();
	// �g�p�L�[�{�[�h�̓o�^
	void RegisterKeyboard();
	// ���L�[�������ꂽ�Ƃ��̏���
	void OnKeyDownArrowDown();
	// ��L�[�������ꂽ�Ƃ��̏���
	void OnKeyDownArrowUp();
	// �X�y�[�X�L�[�������ꂽ�Ƃ��̏���
	void OnKeyDownSpace();

	// ------------------------------
	// �����o�ϐ�
	// ------------------------------
private:
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// �e�N�X�`��
	std::unique_ptr<CommonResources> m_commonResources;
	// ���[�U�[�C���^�[�t�F�[�X���X�g
	std::vector<std::unique_ptr<UserInterface>> m_uiList;
	// �V�[���ύX�t���O
	bool m_isChangeScene = false;
	// �Q�[���f�[�^
	GameData* m_gameData = nullptr;
};