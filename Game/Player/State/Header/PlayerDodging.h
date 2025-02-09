// ----------------------------------------------
// 名前:	PlayerDodging.h
// 内容:	プレイヤーの回避状態
// 作成:	池田
// ----------------------------------------------
#pragma once
#ifndef PLAYER_DODGING_DEFINED
#define PLAYER_DODGING_DEFINED

// インクルード
#include "pch.h"
#include "Interface/IPlayer.h"

// 前方宣言
class Player;

/// <summary>
/// プレイヤーの回避状態
/// </summary>
class PlayerDodging : public IPlayer
{
	// -----------------------------
	// 固定値
	// -----------------------------
private:
	// 回避中の回転時間
	static constexpr float SPIN_TIME = 1.0f;
	// 回避後の回転時間
	static constexpr float SPIN_REST_TIME = 1.5f;
	// 回避中のアニメーション時間
	static constexpr float ANIMATION_TIME = 0.4f;
	// 回避後の硬直時間
	static constexpr float DODGING_RECOVERY_TIME = 1.0f;
	// ローリング終了時間
	static constexpr float ANIMATION_END_TIME = 1.5f;
	// 回避時の摩擦
	static constexpr float DODGE_FUNCTION = 0.93f;
	// 少数〇〇桁以下削除
	static constexpr int TRUNCATION_DIGIT = 4;
	// 回避時の速度
	// 上昇量
	static constexpr float DODGING_SPEED = 1.5f;
	static constexpr float UP_VALUE = 3.0f;
	// 回転量
	static constexpr float ROLLING_ROT = 360.0f;
	// ローリング後の回転角度
	static constexpr float ROLLING_REST_ROT = 30.0f;

	// -----------------------------
	// メンバ関数(公開)
	// -----------------------------
public:
	// デストラクタ
	~PlayerDodging() override;
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
	PlayerDodging(Player* player);
	// キー入力
	void OnKeyPressed(const DirectX::Keyboard::Keys& key) override;
	// キー入力
	void OnKeyDown(const DirectX::Keyboard::Keys& key) override;

	// -----------------------------
	// メンバ関数(非公開)
	// -----------------------------
private:
	// プレイヤーのアニメーション用更新処理
	void UpdateAnimation(float totalTime);
	// ローリングアニメーション
	void RollingAnimation(float totalTime);
	// プレイヤーの移動を適用
	void AdjustCharacterTransition(float totalTime);
	// プレイヤーの移動を適用
	void ApplyPlayerMovement(DirectX::SimpleMath::Vector3& parentPos);

	// -----------------------------
	// メンバ変数
	// -----------------------------
private:
	// 総時間
	float m_totalSeconds;
	// 終了時間
	float m_finishTime;
	// ポジション
	DirectX::SimpleMath::Vector3 m_position;
	// プレイヤーの移動速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// プレイヤー
	Player* m_player;
	// 上昇量
	float m_upValue;
	// 下降量
	float m_downValue;
	// 回転量
	DirectX::SimpleMath::Vector3 m_rollingValue;
};

#endif  // PLAYER_DODGING_DEFINED