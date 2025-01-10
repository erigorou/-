// ---------------------------------------------------------------------------------------
// プレイヤーが回避中かどうかをチェックするクラス		: チュートリアル
// ---------------------------------------------------------------------------------------

#pragma once
#include "pch.h"
#include "Interface/IQuestChecker.h"
#include "Game/Scene/PlayScene.h"
#include "Game/Player/Player.h"

class QuestPlayerAvoid : public IQuestChecker
{
public:
	// テクスチャパスを取得する
	const wchar_t* GetTexturePath() override { return L"Resources/Textures/Quest/QuestUI_Dodg.png"; }


	// チュートリアルのテクスチャパスを取得する
	const wchar_t* GetTutorialTexturePath() override { return L"Resources/Textures/Quest/TutorialDodg.png"; }


	// チェッカーを実行する
	bool ExecuteChecker(PlayScene* playScene) override
	{
		// プレイヤーの取得
		auto player = playScene->GetPlayer();

		// 現在のステートとコンボのステートを取得
		auto currentState = player->GetCurrentState();
		auto attackState = player->GetPlayerDodgingState();

		// プレイヤーのステートが攻撃中かどうか
		return (currentState == attackState);
	};
};