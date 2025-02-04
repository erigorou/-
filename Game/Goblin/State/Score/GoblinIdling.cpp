#include "pch.h"
#include "../Header/GoblinIdling.h"
#include "../../Goblin.h"
#include "Libraries/MyLib/Math.h"

// ---------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="goblin">ゴブリンのポインタ</param>
// ---------------------------------------------
GoblinIdling::GoblinIdling(Goblin* goblin)
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
GoblinIdling::~GoblinIdling()
{
}

// ---------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// ---------------------------------------------
void GoblinIdling::Initialize()
{
	// ランダムにクールタイムを設定する
	m_coolTime = 5.0f;
}

// ---------------------------------------------
/// <summary>
/// 初期化処理（in処理）
/// </summary>
// ---------------------------------------------
void GoblinIdling::PreUpdate()
{
	m_goblin->SetIsAttacking(false);
	m_TotalTime = 0.0f;

	// ランダムにクールタイムを設定する
	m_coolTime = Math::RandomFloat(MIN_COOLTIME, MAX_COOLTIME);
}

// ---------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------
void GoblinIdling::Update(const float& elapsedTime)
{
	// 時間を加算する
	m_TotalTime += elapsedTime;

	// 攻撃ステートに遷移する
	if (m_TotalTime > m_coolTime)
	{
		m_goblin->ChangeState(GoblinState::ATTACKING);
	}
}

// ---------------------------------------------
/// <summary>
/// 初期化処理（out処理）
/// </summary>
// ---------------------------------------------
void GoblinIdling::PostUpdate()
{
}

// ---------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
// ---------------------------------------------
void GoblinIdling::Finalize()
{
}