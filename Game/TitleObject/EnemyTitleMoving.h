// ---------------------------------------------------------
//
// 名前:	EnemyTitleMoving.h
// 内容:	敵の攻撃状態
//			ステートパターンで実装
// 作成;	池田桜輔
//
// ---------------------------------------------------------
#pragma once
#ifndef ENEMY_TITLEMOVING_DEFINED
#define ENEMY_TITLEMOVING_DEFINED
// インクルード
#include "Interface/IState.h"

// 前方宣言
class Particle;

/// <summary>
/// 敵の攻撃状態を定義
/// </summary>
class EnemyTitleMoving : public IState
{
	// -----------------------------
	// 固定値
	// -----------------------------
public:
	// sin波の幅
	static constexpr float AMPLITUDE = 2.0f;
	// sin波の周波数
	static constexpr float FREQUENCY = 1.0f;
	// 最小値
	static constexpr float MINIMAL = 0.01f;
	// カメラの揺れ幅
	static constexpr float SHAKE_POWER = 1.0f;

	// -----------------------------
	// メンバ関数(公開)
	// -----------------------------
public:
	// コンストラクタ
	EnemyTitleMoving(TitleEnemy* enemy);
	// デストラクタ
	~EnemyTitleMoving() override;
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

	// ジャンプ中か
	bool m_isJump;
	// シェイクできるか
	bool m_canShake;
	// シェイクパワー
	float m_shakePower;
};

#endif		// ENEMY_IDLING_DEFINED
