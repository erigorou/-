// ---------------------------------------------------------------------------------------
// 名前:	QuestPlayerAvoid.h
// 内容:	クエストのチェッカー
//			プレイヤーが回避中かを確認する
// 作成:	池田桜輔
// ---------------------------------------------------------------------------------------
// インクルード
#pragma once
#include "pch.h"
#include "Interface/IQuestChecker.h"
#include "Game/Scene/PlayScene.h"
#include "Game/Player/Player.h"

/// <summary>
/// チュートリアルの回避クエスト
/// </summary>
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

		// 現在のステートと回避のステートを取得
		auto currentState = player->GetCurrentState();
		auto attackState = player->GetPlayerDodgingState();

		// プレイヤーのステートが攻撃中かどうか
		return (currentState == attackState);
	};
};