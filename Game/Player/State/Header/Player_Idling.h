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
class Enemy;

class PlayerIdling : public IState
{
public:
	// デストラクタ
	~PlayerIdling() override;
	// 初期化する
	void Initialize(DirectX::Model* model) override;
	// 事前更新する
	void PreUpdate() override;
	// 更新処理する
	void Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos) override;
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
	PlayerIdling(Player* player);

private:
	// 総時間
	float m_totalSeconds;
	// プレイヤー
	Player* m_player;
	// モデル
	DirectX::Model* m_model;
};

#endif  // PLAYER_IDLING_DEFINED