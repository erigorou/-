// -----------------------------------------------------
// 名前:	BossStarting.h
// 内容:	ボスの開始時のステートクラス
// 作成:	池田桜輔
// -----------------------------------------------------
#pragma once
#ifndef BOSS_STARTING_DEFINED
#define BOSS_STARTING_DEFINED
// インクルード
#include "Interface/IState.h"

// 前方宣言
class Boss;
class Player;
class IObject;

/// <summary>
/// ボスの開始時のステート
/// </summary>
class BossStarting : public IState
{
	// ---------------------------
	// 固定値
	// ---------------------------
public:
	// 総時間
	static constexpr float TOTAL_TIME = 4.0f;
	// 遅延時間
	static constexpr float DELAY_TIME = 0.5f;
	// モーション時間
	static constexpr float MOVE_TIME = 2.5f;
	// 開始時の高さ
	static constexpr float START_HEIGHT = 100.0f;
	// エフェクトの高さ
	static constexpr float PLAY_EFFECT_HEIGHT = 5.0f;
	// カメラを揺らす強さ
	static constexpr float SHAKE_POWER = 2.0f;

	// ---------------------------
	// メンバ関数（公開）
	// ---------------------------
public:
	// コンストラクタ
	BossStarting(Boss* boss);
	// デストラクタ
	~BossStarting() override;
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
	// メンバ関数（非公開）
	// ---------------------------
private:
	// アニメーションの更新処理
	void UpdateAnimation();
	// 遅延時間の更新
	void UpdateDelay();
	// 移動の更新
	void UpdateMove();
	// エフェクトを再生する
	void PlayEffect();

	// ---------------------------
	// メンバ変数
	// ---------------------------
private:
	// 位置
	DirectX::SimpleMath::Vector3 m_position;
	// 回転
	float m_angle;
	// 開始の遅延が終了しているか
	bool m_isEndDelay;
	// 総時間
	float m_totalSeconds;
	// 敵
	Boss* m_boss;
	// プレイヤー
	IObject* m_player;
};

#endif
