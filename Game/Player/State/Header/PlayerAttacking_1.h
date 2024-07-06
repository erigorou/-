#pragma once
#ifndef PLAYER_ATTACKING_1_DEFINED
#define PLAYER_ATTACKING_1_DEFINED
// -----------------------------
//
// プレイヤーの待機状態
//
// -----------------------------


#pragma once
#include "pch.h"
#include "Interface/IState.h"

class Player;

class PlayerAttacking_1 : public IState
{


public:
	// 体の境界球を取得
	DirectX::BoundingSphere GetBoundingSphereBody() { return m_boundingSphereBody; }

public:
	// コンストラクタ
	PlayerAttacking_1(Player* player);
	// デストラクタ
	~PlayerAttacking_1() override;
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
	// 体の境界球
	DirectX::BoundingSphere m_boundingSphereBody;
};

#endif  // PLAYER_ATTACKING_1_DEFINED