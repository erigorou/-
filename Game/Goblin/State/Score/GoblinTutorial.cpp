// --------------------------------------------------
//名前:	GoblinTutorial.cpp
//内容:	チュートリアル中のゴブリンステート
//作成:	池田桜輔
// --------------------------------------------------
#include "pch.h"
#include "../Header/GoblinTutorial.h"
#include "../../Goblin.h"

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
}

// ---------------------------------------------
/// <summary>
/// 更新
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------
void GoblinTutorial::Update(const float& elapsedTime)
{
	// 時間を加算する
	m_TotalTime += elapsedTime;
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