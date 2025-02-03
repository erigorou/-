#pragma once
#ifndef PLAYER_ATTACKING_2_DEFINED
#define PLAYER_ATTACKING_2_DEFINED
// -----------------------------
//
// プレイヤーの攻撃状態
//
// -----------------------------

#pragma once
#include "pch.h"
#include "Interface/IPlayer.h"

class Player;

class PlayerAttacking_2 : public IPlayer
{
public:
	// コンストラクタ
	PlayerAttacking_2(Player* player);
	// デストラクタ
	~PlayerAttacking_2() override;
	// 初期化す
	void Initialize() override;
	// 事前更新する
	void PreUpdate() override;
	// 更新処理する
	void Update(const float& elapsedTime) override;
	// 事後更新する
	void PostUpdate() override;
	// 終了処理
	void Finalize() override;

	// キー入力を通知
	void OnKeyPressed(const DirectX::Keyboard::Keys& key) override;

	void OnKeyDown(const DirectX::Keyboard::Keys& key) override;

private:
	// アニメーションの更新
	void UpdateAnimation();

	// 座標
	DirectX::SimpleMath::Vector3 m_position;
	// 速度
	DirectX::SimpleMath::Vector3 m_velocity;

	// １回の攻撃の間隔
	float m_attackElapsedTime;

	// 総時間
	float m_totalSeconds;
	// プレイヤー
	Player* m_player;
	// モデル
	DirectX::Model* m_model;
};

#endif  // PLAYER_ATTACKING_1_DEFINED