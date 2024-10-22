#pragma once
#ifndef GOBLIN_HP
#define GOBLIN_HP
#include "Interface/IHitPoint.h"

#include "pch.h"

class GoblinHP
{
// 固定値
private:
	static constexpr int MAX_HP = 3; // 最大HP


// 共通関数
public:
	GoblinHP() : m_hp(MAX_HP) {}    // コンストラクタ
	~GoblinHP() = default;          // デストラクタ

	int GetMaxHP() const { return MAX_HP; }  // 最大HPの取得
	int GetHP() const { return m_hp; }    // 現在のHPの取得
	void SetHP(int hp) { m_hp = hp; }      // 現在のHPの設定

	// 回復
	void Heal(int hp)	
	{
		m_hp = std::min(m_hp + hp, MAX_HP); 
	}

	// ダメージ
	bool Damage(int hp) 
	{
		m_hp = std::max(m_hp - hp, 0);
		return CheckGoblinDie();
	}


// メンバ関数
private:
	// ゴブリンが死んでいるかどうか
	bool CheckGoblinDie() const { return m_hp <= 0; }



// メンバ変数
private:
	int m_hp;
};


#endif // !GOBLIN_HP