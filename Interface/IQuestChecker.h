#pragma once
#include "pch.h"

class PlayScene;

class IQuestChecker
{
	// ---------------
	// 公開関数
	// ---------------
public:

	// テクスチャパスを取得する
	virtual const wchar_t* GetTexturePath() = 0;

	// チュートリアルのテクスチャパスを取得する
	virtual const wchar_t* GetTutorialTexturePath() = 0;

	// チェッカーを実行する
	virtual bool ExecuteChecker(PlayScene* playScene) = 0;

	virtual ~IQuestChecker() = default;
};