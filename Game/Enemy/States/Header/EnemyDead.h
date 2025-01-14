#pragma once
#ifndef ENEMY_DEAD_DEFINED
#define ENEMY_DEAD_DEFINED
#include "Interface/IState.h"

class Enemy;
class Player;

// 敵の待機ステートを定義
class EnemyDead : public IState
{
// ---------------------------
// 固定値
// ---------------------------
private:
	static constexpr float TOTAL_TIME = 3.0f;


// ---------------------------
// 公開関数
// ---------------------------
public:
	// コンストラクタ
	EnemyDead(Enemy* enemy);
	// デストラクタ
	~EnemyDead() override;
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
// 内部関数
// ---------------------------
private:
	// アニメーションの更新
	void UpdateAnimation();

// ---------------------------
// メンバ変数
// ---------------------------
private:
	// 回転
	float m_angle;

	// 総時間
	float m_totalSeconds;
	// 敵
	Enemy* m_enemy;

	// アニメーション用の回転座標
	float m_tiltAngle;
};

#endif		// ENEMY_IDLING_DEFINED
