#pragma once
#include "pch.h"
#include "Game/UI/UIAnchor.h"
#include "Interface/IAction.h"

class NormalAction;
class TitleLogoAction;
class UserInterface;


class QuestSelectSceneUIManager
{
// ------------------------------
// ���J�֐�
// ------------------------------
public:
	// �R���X�g���N�^
	QuestSelectSceneUIManager();
	// �f�X�g���N�^
	~QuestSelectSceneUIManager();
	// ������
	void Initialize();
	// �X�V
	void Update(const float time);
	// �`��
	void Render();


// ------------------------------
// �����֐�
// ------------------------------
private:
	// ���[�U�[�C���^�[�t�F�[�X�̒ǉ�
	void AddUserInterface(
		const std::string textureName,
		DirectX::SimpleMath::Vector2 position,
		DirectX::SimpleMath::Vector2 scale,
		ANCHOR anchor,
		IAction* action
	);

// ------------------------------
// �����ϐ�
// ------------------------------
private:
	// ���[�U�[�C���^�[�t�F�[�X���X�g
	std::vector<std::unique_ptr<UserInterface>> m_userInterfaceList;
};


