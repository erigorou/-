#pragma once
#ifndef ENEMY_ATTACKING_DEFINED
#define ENEMY_ATTACKING_DEFINED
#include "Interface/IState.h"

class Enemy;
class Player;

// 敵の待機ステートを定義
class Enemy_Attacking : public IState
{
public:
	// 体の境界球を取得
	DirectX::BoundingSphere GetBoundingSphereBody() { return m_boundingSphereBody; }
public:
	// コンストラクタ
	Enemy_Attacking(Enemy* enemy);
	// デストラクタ
	~Enemy_Attacking() override;
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

	// プレイヤーのボディとの当たり判定
	void CheckHitPlayerBody();



private:
	// 回転
	float m_angle;

	// 総時間
	float m_totalSeconds;
	// 敵
	Enemy* m_enemy;
	// モデル
	DirectX::Model* m_model;
	// 体の境界球
	DirectX::BoundingSphere m_boundingSphereBody;
};

#endif		// ENEMY_IDLING_DEFINED
