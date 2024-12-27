#include "pch.h"
#include "../Header/GoblinAttacking.h"
#include "../../Goblin.h"

// コンストラクタ
GoblinAttacking::GoblinAttacking(Goblin* goblin)
	: m_goblin(goblin)
	, m_totalTime(0.0f)
{
}

// 終了処理
GoblinAttacking::~GoblinAttacking()
{
}


// 初期化処理
void GoblinAttacking::Initialize()
{
}


// 事前更新
void GoblinAttacking::PreUpdate()
{
	m_goblin->SetIsAttacking(true);
}


// 更新
void GoblinAttacking::Update(const float& elapsedTime)
{
	// 時間を加算する
	m_totalTime += elapsedTime;
}


// 事後更新
void GoblinAttacking::PostUpdate()
{
	m_goblin->SetIsAttacking(false);
}

// 終了処理
void GoblinAttacking::Finalize()
{
}

// プレイヤーの探索を行う。
void GoblinAttacking::SearchPlayer()
{

}


