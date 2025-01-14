#pragma once
#ifndef ENEMY_IDLING_DEFINED
#define ENEMY_IDLING_DEFINED
#include "Interface/IState.h"

class Enemy;
class Player;

// 敵の待機ステートを定義
class EnemyIdling : public IState
{
// ---------------------------
// 公開関数
// ---------------------------
public:
	// コンストラクタ
	EnemyIdling(Enemy* enemy);
	// デストラクタ
	~EnemyIdling() override;
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
	// 回転
	float m_angle;

	// 総時間
	float m_totalSeconds;
	// 敵
	Enemy* m_enemy;
};

#endif		// ENEMY_IDLING_DEFINED
