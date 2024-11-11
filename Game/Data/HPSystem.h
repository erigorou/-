// ---------------=============================-------------------//
//�@HP�V�X�e��
//  11/11(��)
// ---------------=============================-------------------//


#ifndef HP_SYSTEM
#define HP_SYSTEM

#pragma once
#include "pch.h"


class HPSystem
{
// �A�N�Z�T�֐� //
public:
	float GetMaxHP() const { return m_maxHP; }
	float GetHP() const { return m_hp; }
	void SetHP(float hp) { m_hp = hp; }

// ���J�֐� //
public:
	HPSystem(float max);
	~HPSystem();

	void Heal(float hp);
	bool Damage(float hp);

// �����֐� //
private:
	bool CheckDie();

	float m_maxHP;
	float m_hp;
};

#endif