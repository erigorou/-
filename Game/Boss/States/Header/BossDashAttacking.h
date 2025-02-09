// -----------------------------------------------------
// 名前:	BossDashAttacking.h
// 内容:	ボスの突進ステートを定義
// 作成:	池田桜輔
// -----------------------------------------------------
#pragma once
#ifndef BOSS_DASH_ATTACKING_DEFINED
#define BOSS_DASH_ATTACKING_DEFINED

// インクルード
#include "pch.h"
#include "Interface/IState.h"

// 前方宣言
class Enemy;
class Cudgel;
class Player;

/// <summary>
/// ボスの突進ステート
/// </summary>
class BossDashAttacking : public IState
{
	// ---------------------------
	// 固定値
	// ---------------------------
public:
	// 攻撃の貯めモーションの時間
	static constexpr float CHARGE_TIME = 1.0f;
	// ダッシュする時間
	static constexpr float DASH_TIME = 2.0f;
	// 待機
	static constexpr float WAIT_TIME = 3.5f;
	// 攻撃が終わってから元に戻る時間
	static constexpr float RETURN_TIME = 5.0f;
	// 総時間
	static constexpr float TOTAL_TIME = 5.0f;
	// 最大速度
	static constexpr float MAX_SPEED = 90.0f;
	// sin波の速度
	static constexpr float SIN_SPEED = 10.0f;
	// 傾き(ため)
	static constexpr float TILT_ANGLE = 20.0f;
	// 傾き(ダッシュ)
	static constexpr float TILT_ANGLE_DASH = 40.0f;

	// -----------------------------------
	// メンバ関数（公開）
	// -----------------------------------
public:
	// コンストラクタ
	BossDashAttacking(Boss* boss);
	// デストラクタ
	~BossDashAttacking() override;
	// 初期化する
	void Initialize() override;
	// ステート変更処理(in)
	void PreUpdate() override;
	// 更新処理する
	void Update(const float& elapsedTime) override;
	// ステート更新処理(out)
	void PostUpdate() override;
	// 終了処理
	void Finalize() override;

	// -----------------------------------
	// メンバ関数（非公開）
	// -----------------------------------
private:
	// アクションの更新処理
	void UpdateAction();
	// 攻撃の貯めモーション
	void ChargeAction();
	// ダッシュする
	void DashAction();
	// 待機
	void WaitAction();
	// 元に戻る
	void ReturnAction();

	// -----------------------------------
	// メンバ変数
	// -----------------------------------
private:
	// 回転
	float m_angle;
	// 傾き
	float m_bodyTilt;
	// 攻撃中
	bool m_isAttacking;
	// 回転用行列
	DirectX::SimpleMath::Matrix m_rotMatrix;
	// 移動速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// フレーム時間
	float m_elapsedTime;
	// 総時間
	float m_totalSeconds;
	// 敵
	Boss* m_boss;
	// プレイヤー
	IObject* m_player;
	// 金棒
	Cudgel* m_cudgel;
};

#endif		// BOSS_DASH_ATTACKING_DEFINED
