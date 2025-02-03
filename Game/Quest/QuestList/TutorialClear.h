// ---------------------------------------------------------------------------------------
// プレイヤーが移動しているかをチェックするクラス	: チュートリアル
// ---------------------------------------------------------------------------------------

#pragma once
#include "pch.h"
#include "Interface/IQuestChecker.h"
#include "Game/Scene/PlayScene.h"
#include "Game/EnemyManager/EnemyManager.h"

class TutorialClear : public IQuestChecker
{
public:

	// テクスチャパスを取得する
	const wchar_t* GetTexturePath() override { return nullptr; }

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