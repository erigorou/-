#pragma once
#include "pch.h"
#include "Interface/IQuestChecker.h"
#include "../../Scene/PlayScene.h"

class TutorialEnd : public IQuestChecker
{
private:

public:

	// テクスチャパスを取得する
	const wchar_t* GetTexturePath() override { return L"Resources/Textures/Quest/QuestUI_Boss.png"; }

	// チュートリアルのテクスチャパスを取得する
	const wchar_t* GetTutorialTexturePath() override { return L"Resources/Textures/Quest/TutorialEnd.png"; }

	// チェッカーを実行する
	bool ExecuteChecker(PlayScene* playScene) override
	{
		// 敵マネージャーの取得
		auto enemyManager = playScene->GetEnemyManager();

		// 敵が全滅しているならtrueを返す
		return !enemyManager->IsEnemysAlive();
	};
};