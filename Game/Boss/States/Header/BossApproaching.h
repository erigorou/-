// -----------------------------------------------------
// 名前:	BossApproaching.h
// 内容:	ボスの待機ステートクラス
// 作成:	池田桜輔
// -----------------------------------------------------
#pragma once
#ifndef BOSS_APPROACHING_DEFINED
#define BOSS_APPROACHIGN_DEFINED
// インクルード
#include "Interface/IState.h"

// 前方宣言
class Boss;

/// <summary>
/// 敵の追尾ステート
/// </summary>
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

	// 全体の割合
	static constexpr int TOTAL_RATE = 4;
	// 薙ぎ払いを行う割合
	static constexpr int SWEEPING_RATE = 1;
	// たたきつけを行う割合
	static constexpr int ATTACKING_RATE = 2;
	// 何もしない割合
	static constexpr int IDLING_RATE = 3;

	// ---------------------------
	// メンバ関数(公開)
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

	// --------------------------
	// メンバ関数(非公開)
	// --------------------------
private:
	// アニメーションの更新
	void UpdateAnimation(float elapsedTime);
	// いつ次のステートに移行するかを検知
	void CheckNextState();

	// ---------------------------
	// メンバ変数
	// ---------------------------
private:
	// 座標
	DirectX::SimpleMath::Vector3 m_position;
	// 移動速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// 回転
	float m_angle;
	// 総時間
	float m_totalSeconds;
	// 敵
	Boss* m_boss;
	// プレイヤー
	IObject* m_player;
};

#endif		// Boss_IDLING_DEFINED
