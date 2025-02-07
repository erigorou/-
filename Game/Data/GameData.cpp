// ------------------------------------------------
// 名前:	GameData.cpp
// 内容:	ゲームデータを渡すためのクラス
//			主にシーン間でのデータの受け渡しに使用する
// 作成;	池田桜輔
// ------------------------------------------------

// インクルード
#include "pch.h"
#include "GameData.h"

std::unique_ptr<GameData> GameData::s_data = nullptr;

// ------------------------------------------------
/// <summary>
/// インスタンスの取得
/// </summary>
/// <returns>GameDataのポインタ</returns>
// ------------------------------------------------
GameData* const GameData::GetInstance()
{
	// インスタンスが生成されていない場合
	if (s_data == nullptr)
	{
		// 生成する
		s_data.reset(new GameData());
	}

	// インスタンスを返す
	return s_data.get();
}

// ------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// ------------------------------------------------
GameData::GameData()
	: m_battleResult{ BATTLE_RESULT::NONE }
	, m_questIndex{}
{
}