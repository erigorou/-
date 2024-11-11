// ---------------=============================-------------------//
//　HPシステム
//  11/11(月)
// ---------------=============================-------------------//


#ifndef HP_SYSTEM
#define HP_SYSTEM

#pragma once
#include "pch.h"


class HPSystem
{
// アクセサ関数 //
public:
	float GetMaxHP() const { return m_maxHP; }
	float GetHP() const { return m_hp; }
	void SetHP(float hp) { m_hp = hp; }

// 公開関数 //
public:
	HPSystem(float max);
	~HPSystem();

	void Heal(float hp);
	bool Damage(float hp);

// 内部関数 //
private:
	bool CheckDie();

	float m_maxHP;
	float m_hp;
};

#endif