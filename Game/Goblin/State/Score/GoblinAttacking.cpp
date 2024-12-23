#include "pch.h"
#include "../Header/GoblinAttacking.h"

// コンストラクタ
Goblin::GoblinAttacking::GoblinAttacking(Goblin* goblin)
	: m_goblin(goblin)
	, m_totalTime(0.0f)
{
}

// 終了処理
Goblin::GoblinAttacking::~GoblinAttacking()
{
}


// 初期化処理
void Goblin::GoblinAttacking::Initialize()
{
}


// 事前更新
void Goblin::GoblinAttacking::PreUpdate()
{
	m_goblin->SetIsAttacking(true);
}


// 更新
void Goblin::GoblinAttacking::Update(const float& elapsedTime)
{
	// 時間を加算する
	m_totalTime += elapsedTime;
}


// 事後更新
void Goblin::GoblinAttacking::PostUpdate()
{
	m_goblin->SetIsAttacking(false);
}

// 終了処理
void Goblin::GoblinAttacking::Finalize()
{
}

// プレイヤーの探索を行う。
void Goblin::GoblinAttacking::SearchPlayer()
{

}


