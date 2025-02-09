// --------------------------------------------------------------
//
// 名前:	TutorialButtonAction
// 機能:	チュートリアルに進むボタンのアクション
// 製作:	池田桜輔
//
// --------------------------------------------------------------
#pragma once
// インクルード
#include "pch.h"
#include "Interface/IAction.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Data/GameData.h"

/// <summary>
/// チュートリアルボタンのアクション
/// </summary>
class TutorialButtonAction final : public IAction
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
	static constexpr float INIT_POSITION_Y = 2080.0f;
	// 移動距離
	static constexpr float MOVE_DISTANCE = 800.0f;

	// ------------------------------
	// 公開関数
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

		// チュートリアルボタンの透明度を設定
		if (GameData::GetInstance()->GetSelectStage() != 0)
			result.alpha = 0.5f;

		// 遅延内ならば計算無し
		if (time < DELAY) return result;

		// 秒数を正規化する
		float easing = Math::Clamp(time - DELAY, 0.0f, MOVE_TIME) / MOVE_TIME;
		// 上下にアニメーションさせる
		result.position.x = INIT_POSITION_Y - MOVE_DISTANCE * Easing::easeOutBack(easing);

		// 結果を返す
		return result;
	}
};