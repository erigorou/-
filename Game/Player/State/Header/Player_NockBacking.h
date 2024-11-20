#pragma once

#ifndef PLAYER_NOCKBACKING_DEFINED
#define PLAYER_NOCKBACKING_DEFINED
// -----------------------------
//
// プレイヤーのやられ状態
//
// -----------------------------


#pragma once
#include "pch.h"
#include "Interface/IPlayer.h"

class Player;
class Enemy;

class PlayerNockBacking : public IPlayer
{

// 固定値 *
private:

	static constexpr float NOCKBACKING_TIME = 1.0f;		// やられ全体の時間
	static constexpr float RETURN_TIME		= 2.0f;		// やられから復帰する時間
	static constexpr float END_TIME			= 2.3f;		// やられから復帰する時間

	static constexpr float MAX_UP_VALUE		= 2.0f;	// 上昇値
	static constexpr float RETURN_UP_VALUE	= 1.0f;	// 上昇値
	static constexpr float SPEED = 50.0f;	// 速度


// 公開関数 *
public:
	// デストラクタ
	~PlayerNockBacking() override;
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

	// コンストラクタ
	PlayerNockBacking(Player* player);

	// キー入力
	void OnKeyPressed(const DirectX::Keyboard::Keys& key) override;
	// キー入力
	void OnKeyDown(const DirectX::Keyboard::Keys& key) override;


// 内部関数 *
private:
	void UpdateAnimation();

	void NockBackAnimation();

	void ReturnAnimation();


// 内部変数 *
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