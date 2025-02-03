#pragma once
#include "pch.h"
#include "Interface/IAction.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Libraries/MyLib/Math.h"

// タイトルロゴのアクション
class TitleLogoAction final : public IAction
{
	// ------------------------------
	// 固定値
	// ------------------------------
private:
	// 遅延
	static constexpr float DELAY = 1.5f;
	// 移動時間
	static constexpr float MOVE_TIME = 1.5f;
	// 初期座標
	static constexpr float INIT_POSITION_Y = 200.0f;
	// 移動距離
	static constexpr float MOVE_DISTANCE = 200.0f;

	// ------------------------------
	// 公開関数
	// ------------------------------
public:
	// 実行内容
	ActionParams Execute(ActionParams param, const float time) override
	{
		ActionParams result = param;

		// 遅延内ならば計算無し
		if (time < DELAY) return result;

		// 秒数を正規化する
		float t = Math::Clamp(time - DELAY, 0.0f, MOVE_TIME) / MOVE_TIME;
		// 上下にアニメーションさせる
		result.position.y = MOVE_DISTANCE * Easing::easeOutElastic(t) - INIT_POSITION_Y;

		// 結果を返す
		return result;
	}
};