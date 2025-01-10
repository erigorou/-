// ---------------------------------------------------------------------------------------
// プレイヤーがコンボ中かどうかをチェックするクラス		: チュートリアル
// ---------------------------------------------------------------------------------------


#pragma once
#include "pch.h"
#include "Interface/IQuestChecker.h"
#include "Game/Scene/PlayScene.h"
#include "Game/Player/Player.h"

class QuestPlayerCombo : public IQuestChecker
{
public:

	// テクスチャパスを取得する
	const wchar_t* GetTexturePath() override { return L"Resources/Textures/Quest/QuestUI_Combo.png"; }

	// チュートリアルのテクスチャパスを取得する
	const wchar_t* GetTutorialTexturePath() override { return L"Resources/Textures/Quest/TutorialCombo.png"; }


	// チェッカーを実行する
	bool ExecuteChecker(PlayScene* playScene) override
	{
		// プレイヤーの取得
		auto player = playScene->GetPlayer();

		// 現在のステートとコンボのステートを取得
		auto currentState = player->GetCurrentState();
		auto attackState = player->GetPlayerAttackingState2();

		// プレイヤーのステートが攻撃中かどうか
		return (currentState == attackState);
	};
};