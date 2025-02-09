// ---------------------------------------------------------------------------------------
// 名前:	QuestChangeTarget.h
// 内容:	クエストのチェッカー
//			ターゲットの変更を行ったかを確認する
// 作成:	池田桜輔
// ---------------------------------------------------------------------------------------

// インクルード
#pragma once
#include "pch.h"
#include "Interface/IQuestChecker.h"
#include "Game/Scene/PlayScene.h"


/// <summary>
/// チュートリアルのターゲット変更クエスト
/// </summary>
class QuestChangeTarget : public IQuestChecker
{
public:
	// テクスチャパスを取得する
	const wchar_t* GetTexturePath() override { return L"Resources/Textures/Quest/QuestUI_Target.png"; }

	// チュートリアルのテクスチャパスを取得する
	const wchar_t* GetTutorialTexturePath() override { return L"Resources/Textures/Quest/TutorialChangeTarget.png"; }

	// チェッカーを実行する
	bool ExecuteChecker(PlayScene* playScene) override
	{
		UNREFERENCED_PARAMETER(playScene);

		// キーボードの状態を取得する
		DirectX::Keyboard::State keyboardState = DirectX::Keyboard::Get().GetState();
		DirectX::Keyboard::KeyboardStateTracker keyboardTracker;
		// キーボードステートトラッカーを更新する
		keyboardTracker.Update(keyboardState);

		// spaceが押されたか
		return (keyboardTracker.IsKeyPressed(DirectX::Keyboard::Space));
	};
};