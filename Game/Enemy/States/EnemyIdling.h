#pragma once
#ifndef ENEMY_IDLING_DEFINED
#define ENEMY_IDLING_DEFINED
#include "Interface/IState.h"

class Enemy;

// 敵の待機ステートを定義
class EnemyIdling : public IState
{
public:
	// コンストラクタ
	EnemyIdling(Enemy* enemy);
	// デストラクタ
	~EnemyIdling() override;
	// 初期化する
	void Initialize() override;
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
	// 総時間
	float m_totalSeconds;
	// 終了時間
	float m_finishTime;
	// 敵
	Enemy* m_enemy;
};

#endif		// ENEMY_IDLING_DEFINED
