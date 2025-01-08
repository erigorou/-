#pragma once
#include "pch.h"
#include "Interface/IQuestChecker.h"


#include "Game/Scene/PlayScene.h"
#include "Game/Player/Player.h"

class QuestPlayerAvoid : public IQuestChecker
{
public:
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