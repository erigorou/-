// -----------------------------------------------------
// 名前:	BossApproaching.h
// 内容:	ボスの追尾ステートクラス
// 作成:	池田桜輔
// -----------------------------------------------------
#pragma once
#ifndef BOSS_ATTACKING_DEFINED
#define BOSS_ATTACKING_DEFINED
// インクルード
#include "Interface/IState.h"

// 前方宣言
class Boss;
class Player;

/// <summary>
/// ボスの叩きつけ攻撃ステート
/// </summary>
class BossAttacking : public IState
{
	// ---------------------------
	// 固定値
	// ---------------------------
public:
	// 総時間
	static constexpr float TOTAL_TIME = 5.0f;
	// 敵を追いかける時間
	static constexpr float CHASE_TIME = 1.4f;

	// ---------------------------
	// メンバ関数（公開）
	// ---------------------------
public:
	// コンストラクタ
	BossAttacking(Boss* boss);
	// デストラクタ
	~BossAttacking() override;
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
	// メンバ変数
	// ---------------------------
private:
	// 回転
	float m_angle;
	// 総時間
	float m_totalSeconds;
	// 敵
	Boss* m_boss;
	// プレイヤー
	IObject* m_player;
};

#endif		// BOSS_ATTACKING_DEFINED
