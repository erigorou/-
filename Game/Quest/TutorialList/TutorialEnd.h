#pragma once
#include "pch.h"
#include "Interface/IQuestChecker.h"
#include "../../Scene/PlayScene.h"
#include "../Tutorial/Tutorial.h"


class TutorialEnd : public IQuestChecker
{
private:

public:

	// テクスチャパスを取得する
	const wchar_t* GetTexturePath() override { return nullptr; }

	// チュートリアルのテクスチャパスを取得する
	const wchar_t* GetTutorialTexturePath() override { return L"Resources/Textures/Quest/TutorialEnd.png"; }

	// チェッカーを実行する
	bool ExecuteChecker(PlayScene* playScene) override
	{
		auto tutorial = playScene->GetQuestManager()->GetTutorial();

		// alpha値を減らす
		tutorial->StartAlphaMinus();

		return tutorial->GetTimerIsEnd();
	};
};