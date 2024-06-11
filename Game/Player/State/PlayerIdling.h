#pragma once
#ifndef PLAYER_IDLING_DEFINED
#define PLAYER_IDLING_DEFINED
// -----------------------------
//
// プレイヤーの待機状態
//
// -----------------------------


#pragma once
#include "pch.h"
#include "Interface/IState.h"

class Player;

class PlayerIdling : IState
{
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
	// 描画する
	void Render(
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection) override;
	// 終了処理
	void Finalize() override;

private:
	// 総時間
	float m_totalSeconds;
	// 終了時間
	float m_finishTime;
	// プレイヤー
	Player* m_player;
};

#endif  // PLAYER_IDLING_DEFINED