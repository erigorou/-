#include "pch.h"
#include "../Header/GoblinTutorial.h"
#include "../../Goblin.h"
#include "Libraries/MyLib/Math.h"

// ---------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="goblin">ゴブリンのポインタ</param>
// ---------------------------------------------
GoblinTutorial::GoblinTutorial(Goblin* goblin)
	: 
	m_goblin{ goblin },
	m_TotalTime{}
{
}

// ---------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------------
GoblinTutorial::~GoblinTutorial()
{
}

// ---------------------------------------------
/// <summary>
/// 初期化
/// </summary>
// ---------------------------------------------
void GoblinTutorial::Initialize()
{
	// ランダムにクールタイムを設定する
	m_coolTime = Math::RandomFloat(MIN_COOLTIME, MAX_COOLTIME);
}

// ---------------------------------------------
/// <summary>
/// 初期化(in)
/// </summary>
// ----------------------------------------------
void GoblinTutorial::PreUpdate()
{
	m_goblin->SetIsAttacking(false);
	m_TotalTime = 0.0f;

	// ランダムにクールタイムを設定する
	m_coolTime = Math::RandomFloat(MIN_COOLTIME, MAX_COOLTIME);
}

// ---------------------------------------------
/// <summary>
/// 更新
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------
void GoblinTutorial::Update(const float& elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}

// ---------------------------------------------
/// <summary>
/// 初期化(out)
/// </summary>
// ---------------------------------------------
void GoblinTutorial::PostUpdate()
{
}

// ---------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
// ---------------------------------------------
void GoblinTutorial::Finalize()
{
}