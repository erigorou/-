#pragma once
#ifndef GOBLIN_DEAD
#define GOBLIN_DEAD

#include "pch.h"
#include "Interface/IState.h"
class Goblin;

class GoblinDead : public IState
{

// -----------0-
// 固定値
// -------------
public:
	static constexpr float DEAD_TIME = 2.5f;



// -----------0-
// 公開関数
// -------------
public:

	// コンストラクタ
	GoblinDead(Goblin* goblin);

	// デストラクタ
	~GoblinDead() override;

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


// -------------------
// 内部関数
// -------------------
private:

	// アニメーションの更新処理
	void UpdateAnimation();


// -------------------
// 内部変数
// -------------------
private:

	Goblin* m_goblin;	// 親
	float m_totalTime;	// 経過時間
	float m_scale;		// スケール
	DirectX::SimpleMath::Vector3 m_startPos;	// 開始地点
};


#endif