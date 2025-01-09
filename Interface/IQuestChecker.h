#pragma once
#include "pch.h"

class PlayScene;

class IQuestChecker
{
// ---------------
// ���J�֐�
// ---------------
public:

	// �e�N�X�`���p�X���擾����
	virtual const wchar_t* GetTexturePath() = 0;

	// �`�F�b�J�[�����s����
	virtual bool ExecuteChecker(PlayScene* playScene) = 0;
};