#pragma once
#ifndef GOBLIN_IDLING
#define GOBLIN_IDLING

#include "pch.h"
#include "../../Goblin.h"


class Goblin::GoblinIdling : public IState
{

// 共通
public:

	GoblinIdling(Goblin* goblin);

	~GoblinIdling() override;

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


// メンバ
private:
	Goblin* m_goblin;	// 親
	float m_TotalTime;	// 経過時間
};


#endif