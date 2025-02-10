// --------------------------------------------
//
// 名前:	GoTitleButtonAction
// 機能:	リザルトでのタイトルへ戻るボタンのアクション
// 製作:	池田桜輔
//
// --------------------------------------------

#pragma once
// インクルード
#include "Interface/IAction.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Libraries/MyLib/Math.h"

/// <summary>
/// リザルトでのタイトルへ戻るボタンのアクション
/// </summary>
class GoTitleButtonAction final : public IAction
{
	// ------------------------------
	// 固定値
	// ------------------------------
private:
	// 遅延
	static constexpr float DELAY = 2.0f;
	// 移動時間
	static constexpr float MOVE_TIME = 1.5f;
	// 初期座標
	static constexpr float INIT_POSITION_Y = 800.0f;
	// 移動距離
	static constexpr float MOVE_DISTANCE = -225.0f;


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
		ActionParams result = param;

		// 遅延内ならば計算無し
		if (time < DELAY) return result;

		// 秒数を正規化する
		float easing = Math::Clamp(time - DELAY, 0.0f, MOVE_TIME) / MOVE_TIME;
		// 上下にアニメーションさせる
		result.position.y = MOVE_DISTANCE * Easing::easeOutElastic(easing) + INIT_POSITION_Y;

		// 結果を返す
		return result;
	}
};