#pragma once
#include "pch.h"
#include "Interface/IQuestChecker.h"
#include "../../Scene/PlayScene.h"

class TutorialAttackStart : public IQuestChecker
{
public:

	// テクスチャパスを取得する
	const wchar_t* GetTexturePath() override { return nullptr; }

	// チュートリアルのテクスチャパスを取得する
	const wchar_t* GetTutorialTexturePath() override { return L"Resources/Textures/Quest/TutorialAttackStart.png"; }

	// チェッカーを実行する
	bool ExecuteChecker(PlayScene* playScene) override
	{
		auto tutorial = playScene->GetQuestManager()->GetTutorial();

		// チュートリアルのタイマーをスタート
		tutorial->StartTimer();

		// タイマーが終了しているかどうか
		return tutorial->GetTimerIsEnd();
	};
};