// ----------------------------------------------
// 名前:	PlayerNockBacking
// 内容:	プレイヤーのやられ状態
// 作成:	池田
// ----------------------------------------------
#pragma once
#ifndef PLAYER_NOCKBACKING_DEFINED
#define PLAYER_NOCKBACKING_DEFINED

// インクルード
#include "pch.h"
#include "Interface/IPlayer.h"

// 前方宣言
class Player;

/// <summary>
/// プレイヤーのやられ状態
/// </summary>
class PlayerNockBacking : public IPlayer
{
	// -----------------------------
	// 固定値
	// -----------------------------
private:
	// やられ全体の時間
	static constexpr float NOCKBACKING_TIME = 1.0f;
	// やられから復帰する時間
	static constexpr float RETURN_TIME = 2.0f;
	// やられから復帰する時間
	static constexpr float END_TIME = 2.3f;
	// 上昇値
	static constexpr float MAX_UP_VALUE = 2.0f;
	// 上昇値
	static constexpr float RETURN_UP_VALUE = 1.0f;
	// 速度
	static constexpr float SPEED = 50.0f;

	// -----------------------------
	// メンバ関数(公開)
	// -----------------------------
public:
	// デストラクタ
	~PlayerNockBacking() override;
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
	// コンストラクタ
	PlayerNockBacking(Player* player);
	// キー入力
	void OnKeyPressed(const DirectX::Keyboard::Keys& key) override;
	// キー入力
	void OnKeyDown(const DirectX::Keyboard::Keys& key) override;

	// -----------------------------
	// メンバ関数(非公開)
	// -----------------------------
private:
	// アニメーションの更新
	void UpdateAnimation();
	// ノックバックアニメーション
	void NockBackAnimation();
	// 再帰アニメーション
	void ReturnAnimation();

	// -----------------------------
	// メンバ変数
	// -----------------------------
private:
	// 経過時間
	float m_elapsedTime;
	// 総時間
	float m_totalSeconds;
	// ノックバック方向
	float m_nockBackAngle;
	// 傾き
	DirectX::SimpleMath::Vector3 m_bodyTilt;
	// ポジション
	DirectX::SimpleMath::Vector3 m_position;
	// プレイヤーの移動速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// プレイヤー
	Player* m_player;
	// モデル
	DirectX::Model* m_model;
};

#endif  // PLAYER_DODGING_DEFINED