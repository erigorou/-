// ---------------=============================-------------------//
//　HPシステム
//  11/11(月)
// ---------------=============================-------------------//

#include "pch.h"
#include "HPSystem.h"


// -------------------
// コンストラクタ
// -------------------
HPSystem::HPSystem(float max)
	: m_maxHP(max)
	, m_hp(max)
{
	
}


// -------------------
// デストラクタ
// -------------------
HPSystem::~HPSystem()
{
}


// -------------------
// 回復
// -------------------
void HPSystem::Heal(float hp)
{
	m_hp = std::min(m_hp + hp, m_maxHP);
}


// -------------------
// ダメージ
// -------------------
bool HPSystem::Damage(float hp)
{
	m_hp = std::max(m_hp - hp, 0.0f);
	return CheckDie();
}



// -------------------
// 死亡判定
// -------------------
bool HPSystem::CheckDie()
{
	return m_hp <= 0.0f;
}