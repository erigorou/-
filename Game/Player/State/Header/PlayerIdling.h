// ----------------------------------------------
// 名前:	PlayerIdling.h
// 内容:	プレイヤーの待機状態
// 作成:	池田
// ----------------------------------------------
#pragma once
#ifndef PLAYER_IDLING_DEFINED
#define PLAYER_IDLING_DEFINED

// インクルード
#include "pch.h"
#include "Interface/IPlayer.h"

// 前方宣言
class Player;

/// <summary>
/// プレイヤーの待機状態
/// </summary>
class PlayerIdling : public IPlayer
{
	// -----------------------------
	// メンバ関数(公開)
	// -----------------------------
public:
	// コンストラクタ
	PlayerIdling(Player* player);
	// デストラクタ
	~PlayerIdling() override;
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

	// -----------------------------
	// メンバ関数(非公開)
	// -----------------------------
private:
	// キー入力
	void OnKeyPressed(const DirectX::Keyboard::Keys& key) override;
	// キー入力
	void OnKeyDown(const DirectX::Keyboard::Keys& key) override;

	// -----------------------------
	// メンバ変数
	// -----------------------------
private:
	// 総時間
	float m_totalSeconds;
	// プレイヤー
	Player* m_player;
};

#endif  // PLAYER_IDLING_DEFINED