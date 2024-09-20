#pragma once
#ifndef ENEMY_SWEEPING_DEFINED
#define ENEMY_SWEEPING_DEFINED
#include "Interface/IState.h"

class Enemy;
class Player;
class EasyingFunctions;

// 敵の薙ぎ払いステートを定義
class Enemy_Sweeping : public IState
{
public:
	// 体の境界球を取得
	DirectX::BoundingSphere GetBoundingSphereBody() { return m_boundingSphereBody; }
public:
	// コンストラクタ
	Enemy_Sweeping(Enemy* enemy);
	// デストラクタ
	~Enemy_Sweeping() override;
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
	float m_angle;			// 角度	
	float m_totalSeconds;	// 総時間
	Enemy* m_enemy;				// 敵（ステートの元）	
	DirectX::Model* m_model;	// モデル
	DirectX::BoundingSphere m_boundingSphereBody;	// 体の境界球

	std::unique_ptr<EasyingFunctions> m_easying;	// イージング関数
};

#endif		// ENEMY_SWEEPING_DEFINED
