#pragma once
#include "pch.h"
#include "Interface/IQuestChecker.h"
#include "../../Scene/PlayScene.h"
#include "../Tutorial/Tutorial.h"


class TutorialEnd : public IQuestChecker
{
private:

public:

	// �e�N�X�`���p�X���擾����
	const wchar_t* GetTexturePath() override { return nullptr; }

	// �`���[�g���A���̃e�N�X�`���p�X���擾����
	const wchar_t* GetTutorialTexturePath() override { return L"Resources/Textures/Quest/TutorialEnd.png"; }

	// �`�F�b�J�[�����s����
	bool ExecuteChecker(PlayScene* playScene) override
	{
		auto tutorial = playScene->GetQuestManager()->GetTutorial();

		// alpha�l�����炷
		tutorial->StartAlphaMinus();

		return tutorial->GetTimerIsEnd();
	};
};