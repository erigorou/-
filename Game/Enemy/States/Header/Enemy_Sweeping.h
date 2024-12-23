#pragma once
#ifndef ENEMY_SWEEPING_DEFINED
#define ENEMY_SWEEPING_DEFINED
#include "Interface/IState.h"

class Enemy;
class Player;
class Easing;

// 敵の薙ぎ払いステートを定義
class Enemy_Sweeping : public IState
{
public:

	static const float CHARGE_TIME;	// 攻撃の貯めモーションの時間
	static const float WINDUP_TIME;	// 攻撃する前の時間
	static const float ATTACK_TIME;	// 攻撃する時間
	static const float END_TIME;	// 攻撃（全体）が終了する時間

	static const float ROTATE_ANGLE;	// 攻撃の貯めモーションの角度

	// コンストラクタ
	Enemy_Sweeping(Enemy* enemy);
	// デストラクタ
	~Enemy_Sweeping() override;
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

	// プレイヤーのボディとの当たり判定
	void CheckHitPlayerBody();

private:
	float m_angle;				// 角度	
	float m_totalSeconds;		// 総時間
	Enemy* m_enemy;				// 敵（ステートの元）	

};

#endif		// ENEMY_SWEEPING_DEFINED
