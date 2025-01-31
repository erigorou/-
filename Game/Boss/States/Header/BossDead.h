#pragma once
#ifndef Boss_DEAD_DEFINED
#define Boss_DEAD_DEFINED
#include "Interface/IState.h"

class Boss;
class Player;
class Cudgel;

// 敵の待機ステートを定義
class BossDead : public IState
{
// ---------------------------
// 固定値
// ---------------------------
private:

	// 総時間
	static constexpr float TOTAL_TIME = 3.0f;
	// 傾ける量
	static constexpr float MAX_TILT_ANGLE = -90.0f;
	// カメラを揺らす強さ
	static constexpr float CAMERA_SHAKE_POWER = 1.0f;
	// カメラを揺らすタイミング
	static constexpr float CAMERA_SHAKE_TIMING = -89.0f;

// ---------------------------
// 公開関数
// ---------------------------
public:
	// コンストラクタ
	BossDead(Boss* boss);
	// デストラクタ
	~BossDead() override;
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
	// 敵
	Boss* m_boss;
	// 金棒
	Cudgel* m_cudgel;
	// 回転
	float m_angle;
	// 総時間
	float m_totalSeconds;
	// アニメーション用の回転座標
	float m_tiltAngle;
	// 状態開始時の体の傾きを取得
	float m_startTilt;
};

#endif		// Boss_IDLING_DEFINED
