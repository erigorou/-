#pragma once
#ifndef ENEMY_APPROACHING_DEFINED
#define ENEMY_APPROACHIGN_DEFINED
#include "Interface/IState.h"

class Enemy;
class Particle;

// 敵の待機ステートを定義
class EnemyApproaching : public IState
{
	// ---------------------------
	// 固定値
	// ---------------------------
public:
	static constexpr float MINIMAL = 0.01f;				// 最小値
	static constexpr float NORMALIZE_VELOCITY = 75.0f; 	// 速度の正規化



	// ---------------------------
	// 公開関数
	// ---------------------------
public:
	// コンストラクタ
	EnemyApproaching(Enemy* enemy);
	// デストラクタ
	~EnemyApproaching() override;
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
	// 座標
	DirectX::SimpleMath::Vector3 m_position;
	// 移動速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// 回転
	float m_angle;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_worldMat;

	// 総時間
	float m_totalSeconds;
	// 終了時間
	float m_finishTime;
	// 敵
	Enemy* m_enemy;
	// パーティクル
	Particle* m_particles;

	// サイン波の振幅
	float m_amplitude;
	// サイン波の周波数
	float m_frequency;
	// カメラの揺れの強さ
	float m_shakePower;

};

#endif		// ENEMY_IDLING_DEFINED
