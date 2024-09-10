// -----------------------------------------------
//
// �v���C����UI�̕`��␶�����s���֐�
//
// -----------------------------------------------

#pragma once

#ifndef PLAYSCENE_UI_MANAGER
#define PLAYSCENE_UI_MANAGER

#include "pch.h"
#include <unordered_map>

// �O���錾
class IUserInterface;

class PlaySceneUIManager
{
public:

	// �R���X�g���N�^
	PlaySceneUIManager();
	// �f�X�g���N�^
	~PlaySceneUIManager();
	// �������֐�
	void Initialize();
	// �X�V�֐�
	void Update();
	// �`��֐�
	void Render();
	// �I���֐�
	void Finalize();

	// �ǉ��֐�
	void Add(IUserInterface* ui, std::string key);
	// �폜�֐�
	void Remove(std::string key);

private:
	// UI�̃|�C���^���i�[���郊�X�g
	std::unordered_map<std::string, IUserInterface*> m_uiList;
};

#endif