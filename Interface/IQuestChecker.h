#pragma once
#include "pch.h"

class PlayScene;

class IQuestChecker
{
// ---------------
// ���J�֐�
// ---------------
public:

	// �`�F�b�J�[�����s����
	bool virtual ExecuteChecker(PlayScene* playScene) = 0;
};