#pragma once
#ifndef GOBLIN_ATTACKING
#define GOBLIN_ATTACKING

#include "pch.h"
#include "Interface/IState.h"
class Goblin;

class GoblinAttacking : public IState
{

// 固定値 **
private:
	static constexpr float STATE_TIME = 3.0f;	// 状態の時間

	static constexpr float CHARGE_TIME = 1.0f;	// 待機時間
	static constexpr float ATTACK_TIME = 2.0f;	// 攻撃時間
	static constexpr float RETURN_TIME = 3.0f;	// 戻る時間

	static constexpr float MAX_MINUS_SIZE = 0.35f;
	static constexpr float MAX_Y_POS = 20.0f;

// 公開関数 **
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

// 内部関数 **
private:
	// プレイヤーの探索を行う。
	void SearchPlayer();
	// アニメーションの更新
	void UpdateAnimation();

	void ChargeAnimation();
	void AttackAnimation();
	void ReturnAnimation();

private:
	Goblin* m_goblin;	// 親
	float m_totalTime;	// 経過時間

	float m_angle;	// 角度
	DirectX::SimpleMath::Matrix m_rotMatrix;		// 移動用の回転行列
	DirectX::SimpleMath::Vector3 m_position;		// 位置
	DirectX::SimpleMath::Vector3 m_moveValue;		// 移動量
	DirectX::SimpleMath::Vector3 m_movePosition;	// 移動位置
};



#endif	// GOBLIN_ATTACKING