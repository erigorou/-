// --------------------------------------------------
//名前:	GoblinAttacking.h
//内容:	ゴブリンの攻撃状態
//作成:	池田桜輔
// --------------------------------------------------
#pragma once
#ifndef GOBLIN_ATTACKING
#define GOBLIN_ATTACKING
// インクルード
#include "pch.h"
#include "Interface/IState.h"

// 前方宣言
class Goblin;

/// <summary>
/// ゴブリンの攻撃状態
/// </summary>
class GoblinAttacking : public IState
{
	//--------------------
	// 固定値
	// --------------------
private:
	// 状態の時間
	static constexpr float STATE_TIME = 3.0f;
	// 待機時間
	static constexpr float CHARGE_TIME = 1.0f;
	// 攻撃時間
	static constexpr float ATTACK_TIME = 2.0f;
	// 戻る時間
	static constexpr float RETURN_TIME = 3.0f;
	// 最も小さいときのサイズ
	static constexpr float MAX_MINUS_SIZE = 0.35f;
	// 最大飛べる高さ
	static constexpr float MAX_Y_POS = 20.0f;

	// --------------------
	// メンバ関数(公開)
	// --------------------
public:
	// コンストラクタ
	GoblinAttacking(Goblin* goblin);
	// デストラクタ
	~GoblinAttacking() override;
	// 初期化する
	void Initialize() override;
	// 事前更新する
	void PreUpdate() override;
	// 更新する
	void Update(const float& elapsedTime) override;
	// 事後更新する
	void PostUpdate() override;
	// 後処理を行う
	void Finalize() override;

	// --------------------
	// メンバ関数(非公開)
	// --------------------
private:
	// プレイヤーの探索を行う。
	void SearchPlayer();
	// アニメーションの更新
	void UpdateAnimation();

	void ChargeAnimation();
	void AttackAnimation();
	void ReturnAnimation();

private:
	// 親
	Goblin* m_goblin;
	// 経過時間
	float m_totalTime;
	// 角度
	float m_angle;
	// 移動用の回転行列
	DirectX::SimpleMath::Matrix m_rotMatrix;
	// 位置
	DirectX::SimpleMath::Vector3 m_position;
	// 移動量
	DirectX::SimpleMath::Vector3 m_moveValue;
	// 移動位置
	DirectX::SimpleMath::Vector3 m_movePosition;
};
#endif