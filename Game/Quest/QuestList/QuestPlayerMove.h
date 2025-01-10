// ---------------------------------------------------------------------------------------
// プレイヤーが移動しているかをチェックするクラス	: チュートリアル
// ---------------------------------------------------------------------------------------

#pragma once
#include "pch.h"
#include "Interface/IQuestChecker.h"
#include "Game/Scene/PlayScene.h"
#include "Game/Player/Player.h"

class QuestPlayerMove : public IQuestChecker
{
public:

	// テクスチャパスを取得する
	const wchar_t* GetTexturePath() override { return L"Resources/Textures/Quest/QuestUI_Move.png"; }

	// チュートリアルのテクスチャパスを取得する
	const wchar_t* GetTutorialTexturePath() override { return L"Resources/Textures/Quest/TutorialMove.png"; }

	// チェッカーを実行する
	bool ExecuteChecker(PlayScene* playScene) override
	{
		// プレイヤーの取得
		auto player = playScene->GetPlayer();

		// プレイヤーが移動しているか
		return (player->GetinputVector() != DirectX::SimpleMath::Vector2::Zero);
	};
};