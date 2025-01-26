#pragma once
#ifndef ENEMY_TITLEMOVING_DEFINED
#define ENEMY_TITLEMOVING_DEFINED
#include "Interface/IState.h"

class Enemy;
class Particle;

// 敵の待機ステートを定義
class EnemyTitleMoving : public IState
{


public:

	// コンストラクタ
	EnemyTitleMoving(TitleEnemy* enemy);
	// デストラクタ
	~EnemyTitleMoving() override;
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
	static constexpr float MINIMAL		= 0.01f;
	static constexpr float COOL_TIME	= 0.5f;

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
	TitleEnemy* m_enemy;


	// サイン波の振幅
	float m_amplitude;
	// サイン波の周波数
	float m_frequency;
	// ジャンプ中か
	bool m_isJump;
	// シェイクできるか
	bool m_canShake;
	// シェイクパワー
	float m_shakePower;

};

#endif		// ENEMY_IDLING_DEFINED
