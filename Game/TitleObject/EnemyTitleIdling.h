// ---------------------------------------------------------
//
// 名前:	EnemyTitleIdling.h
// 内容:	敵の待機状態
//			ステートパターンで実装
// 作成;	池田桜輔
//
// ---------------------------------------------------------
#pragma once
#ifndef ENEMY_TITLEIDLING_DEFINED
#define ENEMY_TITLEIDLING_DEFINED
// インクルード
#include "Interface/IState.h"

// 前方宣言
class TitleEnemy;


/// <summary>
/// 敵の待機状態を定義
/// </summary>
class EnemyTitleIdling : public IState
{
	// -----------------------------
	// メンバ関数(公開)
	// -----------------------------
public:
	// コンストラクタ
	EnemyTitleIdling(TitleEnemy* enemy);
	// デストラクタ
	~EnemyTitleIdling() override;
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
	// メンバ変数
	// -----------------------------

private:
	// 座標
	DirectX::SimpleMath::Vector3 m_position;
	// 移動速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// 回転
	float m_angle;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_worldMat;
	// 総時間
	float m_totalSeconds;
	// 敵
	TitleEnemy* m_enemy;
};

#endif		// ENEMY_IDLING_DEFINED
