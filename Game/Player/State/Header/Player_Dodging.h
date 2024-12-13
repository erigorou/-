#pragma once
#ifndef PLAYER_DODGING_DEFINED
#define PLAYER_DODGING_DEFINED
// -----------------------------
//
// プレイヤーの待機状態
//
// -----------------------------


#pragma once
#include "pch.h"
#include "Interface/IPlayer.h"

class Player;
class Enemy;

class PlayerDodging : public IPlayer
{
// 定数
private:
	
	static constexpr float DODGING_TIME = 1.0f;			// 回避全体の時間
	static constexpr float ROWLING_END_TIME = 0.7f;		// ローリング終了時間
	static constexpr float ROWLING_TIME = 0.3f;			// 回避中のローリング時間


	static constexpr float DODGE_FUNCTION	= 0.9f;		// 回避時の摩擦
	static constexpr int   TRUNCATION_DIGIT = 4;		// 少数〇〇桁以下削除

	static constexpr float DODGING_SPEED = 1.5f;		// 回避時の速度

// 公開関数
public:
	// デストラクタ
	~PlayerDodging() override;
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
	PlayerDodging(Player* player);

	// キー入力
	void OnKeyPressed(const DirectX::Keyboard::Keys& key) override;
	// キー入力
	void OnKeyDown(const DirectX::Keyboard::Keys& key) override;

// 内部関数
private:
	// プレイヤーのアニメーション用更新処理
	void UpdateAnimation(float totalTime);
	// プレイヤーの移動を適用
	void ApplyPlayerMovement(DirectX::SimpleMath::Vector3& parentPos);


// 内部変数
private:
	// 総時間
	float m_totalSeconds;
	// 終了時間
	float m_finishTime;

	// ポジション
	DirectX::SimpleMath::Vector3 m_position;
	// プレイヤーの移動速度
	DirectX::SimpleMath::Vector3 m_velocity;
	//
	
	// プレイヤー
	Player* m_player;
	// モデル
	DirectX::Model* m_model;


	float m_upValue;	// 上昇量
	float m_downValue;	// 下降量

};

#endif  // PLAYER_DODGING_DEFINED