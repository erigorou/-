#pragma once
#ifndef PLAYER_ATTACKING_4_DEFINED
#define PLAYER_ATTACKING_4_DEFINED
// -----------------------------
//
// プレイヤーの待機状態
//
// -----------------------------


#pragma once
#include "pch.h"
#include "Interface/IPlayer.h"

class Player;

class PlayerAttacking_4 : public IPlayer
{
// 固定値
public:
	static constexpr float DOWN_TIME = 0.4f;

// 公開関数
public:
	// コンストラクタ
	PlayerAttacking_4(Player* player);
	// デストラクタ
	~PlayerAttacking_4() override;
	// 初期化する
	void Initialize(DirectX::Model* model) override;
	// 事前更新する
	void PreUpdate() override;
	// 更新処理する
	void Update(const float& elapsedTime) override;
	// 事後更新する
	void PostUpdate() override;
	// 描画する
	void Render(
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection) override;
	// 終了処理
	void Finalize() override;

	// キー入力
	void OnKeyPressed(const DirectX::Keyboard::Keys& key) override;
	// キー入力
	void OnKeyDown(const DirectX::Keyboard::Keys& key) override;


// 内部関数
private:
	void UpdateAnimation();


// 内部変数
private:
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

	// アニメーション用 ///
	float m_upValue;	// 上昇値
	float m_downValue;	// 下降値
};

#endif  // PLAYER_ATTACKING_1_DEFINED