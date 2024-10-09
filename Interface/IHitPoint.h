#pragma once

#ifndef I_HITPOINT
#define I_HITPOINT

#include "pch.h"

class IHitPoint
{
public:
	// ダメージを受ける
	virtual bool Damage(int damage) = 0;

	// ヒールを受ける
	virtual void Heal(int heal) = 0;

	// HPの取得
	virtual int GetHP() const = 0;

	// 最大HPの取得
	virtual int GetMaxHP() const = 0;
};

#endif // !I_HITPOINT
