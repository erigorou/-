
#pragma once
#ifndef ENEMY_DASH_ATTACKING_DEFINED
#define ENEMY_DASH_ATTACKING_DEFINED
#include "pch.h"
#include "Interface/IState.h"


class Enemy;
class Player;

// 敵の待機ステートを定義
class EnemyDashAttacking : public IState
{
// 固定値 *
public:
	static constexpr float CHARGE_TIME	= 1.0f;	// 攻撃の貯めモーションの時間
	static constexpr float DASH_TIME	= 2.0f;	// ダッシュする時間
	static constexpr float WAIT_TIME	= 3.5f;	// 待機
	static constexpr float RETURN_TIME	= 5.0f;	// 攻撃が終わってから元に戻る時間
	static constexpr float TOTAL_TIME	= 5.0f;	// 総時間

	static constexpr float MAX_SPEED = 100.0f; // 最大速度


// 公開関数 *
public:
	// コンストラクタ
	EnemyDashAttacking(Enemy* enemy);
	// デストラクタ
	~EnemyDashAttacking() override;
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

// 内部関数　*
private:
	void UpdateAction();

	////// 行動系

	// 攻撃の貯めモーション
	void ChargeAction();
	// ダッシュする
	void DashAction();
	// 待機
	void WaitAction();
	// 元に戻る
	void ReturnAction();


// 内部変数　*
private:
	// 回転
	float m_angle;
	// 傾き
	float m_bodyTilt;
	// 攻撃中
	bool m_isAttacking;

	DirectX::SimpleMath::Matrix m_rotMatrix;
	DirectX::SimpleMath::Vector3 m_velocity;

	// フレーム時間
	float m_elapsedTime;
	// 総時間
	float m_totalSeconds;
	// 敵
	Enemy* m_enemy;
};

#endif		// ENEMY_DASH_ATTACKING_DEFINED
