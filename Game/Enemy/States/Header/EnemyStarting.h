#pragma once
#ifndef ENEMY_STARTING_DEFINED
#define ENEMY_STARTING_DEFINED
#include "Interface/IState.h"

class Enemy;
class Player;

// 敵の待機ステートを定義
class EnemyStarting : public IState
{
public:
	static constexpr float TOTAL_TIME = 4.0f;
	static constexpr float DELAY_TIME = 0.5f;
	static constexpr float MOVE_TIME  = 2.5f;
	static constexpr float START_HEIGHT = 100.0f;
	static constexpr float PLAY_EFFECT_HEIGHT = 5.0f;

public:
	// コンストラクタ
	EnemyStarting(Enemy* enemy);
	// デストラクタ
	~EnemyStarting() override;
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


private:
	void UpdateAnimation();
	void UpdateDelay();
	void UpdateMove();
	void PlayEffect();

private:
	// 位置
	DirectX::SimpleMath::Vector3 m_position;
	// 回転
	float m_angle;

	bool m_isEndDelay;

	// 総時間
	float m_totalSeconds;
	// 敵
	Enemy* m_enemy;
};

#endif		// ENEMY_IDLING_DEFINED
