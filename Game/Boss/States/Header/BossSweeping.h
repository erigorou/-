#pragma once
#ifndef BOSS_SWEEPING_DEFINED
#define BOSS_SWEEPING_DEFINED
#include "Interface/IState.h"

class Boss;
class Player;
class Easing;

// 敵の薙ぎ払いステートを定義
class BossSweeping : public IState
{
	// ---------------------------
	// 固定値
	// ---------------------------
public:
	// 攻撃の貯めモーションの時間
	static constexpr float CHARGE_TIME = 0.8f;
	// 攻撃する前の時間
	static constexpr float WINDUP_TIME = 0.9f;
	// 攻撃する時間
	static constexpr float ATTACK_TIME = 1.5f;
	// 攻撃（全体）が終了する時間
	static constexpr float END_TIME = 2.3f;
	// 攻撃の貯めモーションの角度
	static constexpr float ROTATE_ANGLE = 20.0f;


// ---------------------------
// メンバ関数（公開）
// ---------------------------
public:
	// コンストラクタ
	BossSweeping(Boss* boss);
	// デストラクタ
	~BossSweeping() override;
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
// メンバ関数（非公開）
// ---------------------------
private:
	// アニメーションの更新
	void UpdateAnimation();
	// ためモーションの更新
	void UpdateChargeMotion();
	// 薙ぎ払いモーションの更新
	void UpdateSweepMotion();



// ---------------------------
// メンバ変数
// ---------------------------
private:
	// 角度
	float m_angle;
	// 目標の角度
	float m_targetAngle;
	// 総時間
	float m_totalSeconds;
	// 敵
	Boss* m_boss;

};

#endif		// BossSweeping_DEFINED
