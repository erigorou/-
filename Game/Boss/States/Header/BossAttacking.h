#pragma once
#ifndef BossATTACKING_DEFINED
#define BossATTACKING_DEFINED
#include "Interface/IState.h"

class Boss;
class Player;

// 敵の待機ステートを定義
class BossAttacking : public IState
{
// ---------------------------
// 固定値
// ---------------------------
public:
	// 総時間
	static constexpr float TOTAL_TIME = 5.0f;
	// 敵を追いかける時間
	static constexpr float CHASE_TIME = 1.4f;


// ---------------------------
// メンバ関数（公開）
// ---------------------------
public:
	// コンストラクタ
	BossAttacking(Boss* boss);
	// デストラクタ
	~BossAttacking() override;
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

#endif		// BossATTACKING_DEFINED
