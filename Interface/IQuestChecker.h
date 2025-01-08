#pragma once
#include "pch.h"

class PlayScene;

class IQuestChecker
{
// ---------------
// 公開関数
// ---------------
public:

	// チェッカーを実行する
	bool virtual ExecuteChecker(PlayScene* playScene) = 0;
};