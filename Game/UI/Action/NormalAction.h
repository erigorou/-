// --------------------------------------------
//
// 名前:	NormalAction
// 機能:	何もしないアクション
// 製作:	池田桜輔
//
// --------------------------------------------

#pragma once
// インクルード
#include "Interface/IAction.h"

/// <summary>
/// 何もしないアクション
/// </summary>
class NormalAction final : public IAction
{
	// ------------------------------
	// メンバ関数
	// ------------------------------
public:
	// ---------------------------------------------------
	/// <summary>
	/// アクションの実行
	/// </summary>
	/// <param name="param">パラメーター</param>
	/// <param name="time">時間</param>
	/// <returns>アクション後のパラメーター</returns>
	// ---------------------------------------------------
	ActionParams Execute(ActionParams param, const float time) override
	{
		// アクション特になし
		UNREFERENCED_PARAMETER(time);
		return param; 
	}
};