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
	
	static constexpr float SPIN_TIME = 1.0f;			// 回避中の回転時間
	static constexpr float SPIN_REST_TIME = 1.5f;		// 回避後の回転時間

	static constexpr float ANIMATION_TIME = 0.4f;				// 回避中のアニメーション時間
	static constexpr float DODGING_RECOVERY_TIME = 1.0f;	// 回避後の硬直時間
	static constexpr float ANIMATION_END_TIME = 1.5f;			// ローリング終了時間

	static constexpr float DODGE_FUNCTION	= 0.93f;	// 回避時の摩擦
	static constexpr int   TRUNCATION_DIGIT = 4;		// 少数〇〇桁以下削除
	static constexpr float DODGING_SPEED = 1.5f;		// 回避時の速度
	static constexpr float UP_VALUE = 3.0f;				// 上昇量
	static constexpr float ROLLING_ROT = 360.0f;		// 回転量
	static constexpr float ROLLING_REST_ROT = 30.0f;	// ローリング後の回転角度

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
	void RollingAnimation(float totalTime);
	void AdjustCharacterTransition(float totalTime);
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

	DirectX::SimpleMath::Vector3 m_rollingValue;
};

#endif  // PLAYER_DODGING_DEFINED