#pragma once
#ifndef GOBLIN_ATTACKING
#define GOBLIN_ATTACKING

#include "pch.h"
#include "Interface/IState.h"
class Goblin;

class GoblinAttacking : public IState
{
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



private:
	Goblin* m_goblin;			// 親
	float m_totalTime;			// 経過時間

	
};



#endif	// GOBLIN_ATTACKING