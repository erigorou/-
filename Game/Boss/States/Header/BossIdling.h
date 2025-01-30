#pragma once
#ifndef ENEMY_IDLING_DEFINED
#define ENEMY_IDLING_DEFINED
#include "Interface/IState.h"

class Boss;
class Player;

// 敵の待機ステートを定義
class BossIdling : public IState
{
// ---------------------------
// 固定値
// ---------------------------
public:
	// 総時間
	static constexpr float TOTAL_TIME = 1.0f;
	// 距離
	static constexpr float FAR_DISTANCE = 20.0f;
	// 全体の割合
	static constexpr int TOTAL_RATE = 10;
	// 薙ぎ払いを行う割合
	static constexpr int SWEEPING_RATE = 3;
	// たたきつけを行う割合
	static constexpr int ATTACKING_RATE = 7;
	// ダッシュ攻撃を行う割合
	static constexpr int DASH_ATTACK_RATE = 8;
	// 何もしない割合
	static constexpr int IDLING_RATE = 10;


// ---------------------------
// メンバ関数（公開）
// ---------------------------
public:
	// コンストラクタ
	BossIdling(Boss* boss);
	// デストラクタ
	~BossIdling() override;
	// 初期化する
	void Initialize() override;
	// 事前更新する
	void PreUpdate() override;
	// 更新処理する
	void Update(const float& elapsedTime) override;
	// 事後更新する
	void PostUpdate() override;
	// 終了処理
	void Finalize() override;

// ---------------------------
// メンバ関数(非公開)
// ---------------------------
private:
	// アニメーションの更新
	void UpdateAnimation();
	// 次のステートに移行するかを検知
	void CheckNextState();

// ---------------------------
// メンバ変数
// ---------------------------
private:
	// 回転
	float m_angle;

	// 総時間
	float m_totalSeconds;
	// 敵
	Boss* m_boss;
};

#endif		// ENEMY_IDLING_DEFINED
