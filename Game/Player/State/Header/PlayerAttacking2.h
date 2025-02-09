// ----------------------------------------------
// 名前:	PlayerAttacking2.h
// 内容:	プレイヤーの攻撃状態
// 作成:	池田
// ----------------------------------------------
#pragma once
#ifndef PLAYER_ATTACKING2_DEFINED
#define PLAYER_ATTACKING2_DEFINED

// インクルード
#include "pch.h"
#include "Interface/IPlayer.h"

// 前方宣言
class Player;

/// <summary>
/// プレイヤーの攻撃状態
/// </summary>
class PlayerAttacking2 : public IPlayer
{
	// -----------------------------
	// 固定値
	// -----------------------------
public:
	// 初期回転角
	static  constexpr float INIT_ROT = 40.0f;
	// 最終回転角
	static  constexpr float END_ROT = 80.0f;


	// -----------------------------
	// メンバ関数(公開)
	// -----------------------------
public:
	// コンストラクタ
	PlayerAttacking2(Player* player);
	// デストラクタ
	~PlayerAttacking2() override;
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
	// キー入力を通知
	void OnKeyDown(const DirectX::Keyboard::Keys& key) override;

	// -----------------------------
	// メンバ関数(非公開)
	// -----------------------------
private:
	// アニメーションの更新
	void UpdateAnimation();

	// -----------------------------
	// メンバ変数
	// -----------------------------
private:
	// 座標
	DirectX::SimpleMath::Vector3 m_position;
	// 速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// 総時間
	float m_totalSeconds;
	// プレイヤー
	Player* m_player;
};

#endif  // PLAYERATTACKING2_DEFINED