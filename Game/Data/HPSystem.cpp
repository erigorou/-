// ---------------=============================-------------------//
//　HPシステム
//  11/11(月)
// ---------------=============================-------------------//

#include "pch.h"
#include "HPSystem.h"

// ---------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="max">最大HP</param>
// ---------------------------------
HPSystem::HPSystem(float max)
	: m_maxHP(max)
	, m_hp(max)
{
}

// ---------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------
HPSystem::~HPSystem()
{
}

// ---------------------------------
/// <summary>
/// 回復
/// </summary>
// ---------------------------------
void HPSystem::Heal(float hp)
{
	m_hp = std::min(m_hp + hp, m_maxHP);
}


// ---------------------------------
/// <summary>
/// ダメージ
/// </summary>
//---------------------------------
bool HPSystem::Damage(float hp)
{
	m_hp = std::max(m_hp - hp, 0.0f);
	return CheckDie();
}

// ---------------------------------
/// <summary>
/// 死亡判定
/// </summary>
/// <returns>死亡しているか</returns>
// ---------------------------------
bool HPSystem::CheckDie()
{
	return m_hp <= 0.0f;
}