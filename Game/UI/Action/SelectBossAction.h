// ------------------------------------------------------------------------
//
// 名前:	SelectBossAction
// 機能:	セレクトシーンでボス戦を選択したときのアクション
// 制作:	池田桜輔
//
// ------------------------------------------------------------------------

#pragma once
// インクルード
#include "Interface/IAction.h"
#include "Game/Scene/Screen.h"
#include "Game/Data/GameData.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/EasingFunctions.h"

/// <summary>
/// セレクトシーンでボス戦を選択したときのアクション
/// </summary>
class SelectBossAction final : public IAction
{
	// -------------------------------
	// 定数
	// -------------------------------
private:
	// ボス戦のステージ番号
	int BOSS_STAGE_NO = 1;

	// アクションの実行時間
	static constexpr float ACTION_TIME = 0.25f;

	// Y軸の位置
	static constexpr float Y_POSITION = Screen::TOP + 500.0f;

	// 開始地点と終了地点
	static constexpr DirectX::SimpleMath::Vector2 START_POSITION = DirectX::SimpleMath::Vector2(Screen::LEFT, Screen::TOP + Y_POSITION);
	static constexpr DirectX::SimpleMath::Vector2 END_POSITION = DirectX::SimpleMath::Vector2(Screen::LEFT + 50, Y_POSITION);


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
		// 時間を無視
		UNREFERENCED_PARAMETER(time);
		// 移動する
		Move(param);
		// 色を変更する
		ChangeColor(param);
		// パラメーターを返す
		return param;
	}

	// ------------------------------
	// メンバ関数（非公開）
	// ------------------------------
private:
	/// <summary>
	/// ステージを選択されたときにUIを移動させる
	/// </summary>
	/// <param name="param">パラメーター</param>
	void Move(ActionParams& param)
	{
		// ゲームデータにアクセス
		auto gameData = GameData::GetInstance();
		// ボス戦を選択したときの時間を取得
		float selectTime = gameData->GetBossSelectTime();
		// 時間進行を正規化（0.0〜1.0）
		float ease = std::min(selectTime / ACTION_TIME, 1.0f);
		// イージングを適用して移動
		float easedTime = Easing::easeInOutQuad(ease);
		// 補間して現在位置を算出	
		Vector2 currentPos = Math::LerpVector(
			START_POSITION,
			END_POSITION,
			easedTime
		);
		// パラメーターを更新
		param.position = currentPos;
	}

	/// <summary>
	/// 選択しているステージによって色の明るさに変更を加える
	/// </summary>
	/// <param name="param">パラメーター</param>
	void ChangeColor(ActionParams& param)
	{
		// ゲームデータにアクセス
		auto gameData = GameData::GetInstance();
		// 選択しているステージのインデックスを取得
		int stageIndex = gameData->GetSelectStage();

		// 選択ているステージによって色を変更する
		param.color *= stageIndex == BOSS_STAGE_NO ?
			DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f) :
			DirectX::SimpleMath::Vector3(0.5f, 0.5f, 0.5f);
	}
};