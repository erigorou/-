#include "pch.h"
#include "../Header/GoblinIdling.h"
#include "../../Goblin.h"
#include "Libraries/MyLib/Math.h"


// -------------------
// コンストラクタ
// -------------------
GoblinIdling::GoblinIdling(Goblin* goblin)
	: m_goblin(goblin)
	, m_TotalTime(0.0f)
{
}


// -------------------
// デストラクタ
// -------------------
GoblinIdling::~GoblinIdling()
{
}


// -------------------
// 初期化する
// -------------------
void GoblinIdling::Initialize()
{
	// ランダムにクールタイムを設定する
	m_coolTime = Math::RandomFloat(MIN_COOLTIME, MAX_COOLTIME);
}



// -------------------
// 初期化(in)
// -------------------
void GoblinIdling::PreUpdate()
{
	m_goblin->SetIsAttacking(false);
	m_TotalTime = 0.0f;

	// ランダムにクールタイムを設定する
	m_coolTime = Math::RandomFloat(MIN_COOLTIME, MAX_COOLTIME);

}



// -------------------
// 更新する
// -------------------
void GoblinIdling::Update(const float& elapsedTime)
{
	// 時間を加算する
	m_TotalTime += elapsedTime;

	// 攻撃ステートに遷移する
	if (m_TotalTime > m_coolTime)
	{
		m_goblin->ChangeState(m_goblin->GetAttacking());
	}
}


// -------------------
// 初期化(out)
// -------------------
void GoblinIdling::PostUpdate()
{
}


// -------------------
// 後処理を行う
// -------------------
void GoblinIdling::Finalize()
{
}