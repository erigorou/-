#pragma once
#ifndef BOSS_APPROACHING_DEFINED
#define BOSS_APPROACHIGN_DEFINED
#include "Interface/IState.h"

class Boss;

// 敵の待機ステートを定義
class BossApproaching : public IState
{
	// ---------------------------
	// 固定値
	// ---------------------------
public:
	// 総秒数
	static constexpr float TOTAL_TIME = 2.0f;
	// 最小値
	static constexpr float MINIMAL = 0.01f;
	// 速度の正規化用
	static constexpr float NORMALIZE_VELOCITY = 75.0f;
	// サイン波の振幅
	static constexpr float AMPLITUDE = 1.0f;
	// サイン波の周波数
	static constexpr float FREQUENCY = 1.0f;
	// カメラの揺れの強さ
	static constexpr float SHAKE_POWER = 1.0f;
	// プレイヤーとの最小距離
	static constexpr float MIN_DISTANCE = 20.0f;


	// ---------------------------
	// 公開関数
	// ---------------------------
public:
	// コンストラクタ
	BossApproaching(Boss* Boss);
	// デストラクタ
	~BossApproaching() override;
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
	Boss* m_boss;

};

#endif		// Boss_IDLING_DEFINED
