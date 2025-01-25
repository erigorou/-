#include "pch.h"
#include "../Header/GoblinTutorial.h"
#include "../../Goblin.h"
#include "Libraries/MyLib/Math.h"


// -------------------
// コンストラクタ
// -------------------
GoblinTutorial::GoblinTutorial(Goblin* goblin)
	: m_goblin(goblin)
	, m_TotalTime(0.0f)
{
}


// -------------------
// デストラクタ
// -------------------
GoblinTutorial::~GoblinTutorial()
{
}


// -------------------
// 初期化する
// -------------------
void GoblinTutorial::Initialize()
{
	// ランダムにクールタイムを設定する
	m_coolTime = Math::RandomFloat(MIN_COOLTIME, MAX_COOLTIME);
}



// -------------------
// 初期化(in)
// -------------------
void GoblinTutorial::PreUpdate()
{
	m_goblin->SetIsAttacking(false);
	m_TotalTime = 0.0f;

	// ランダムにクールタイムを設定する
	m_coolTime = Math::RandomFloat(MIN_COOLTIME, MAX_COOLTIME);

}



// -------------------
// 更新する
// -------------------
void GoblinTutorial::Update(const float& elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}


// -------------------
// 初期化(out)
// -------------------
void GoblinTutorial::PostUpdate()
{
}


// -------------------
// 後処理を行う
// -------------------
void GoblinTutorial::Finalize()
{
}