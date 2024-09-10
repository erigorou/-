#pragma once
#include "pch.h"

#include "Game/Enemy/Enemy.h"

class Enemy_HP
{
public:

	// HPを取得する処理
	int  GetHP() const { return m_hp; }
	// HPを設定する処理
	void SetHP(const int set) { m_hp = set; }

	// HPを減らす処理
	void ReduceHP(const int reduce) { m_hp -= reduce; }
	// HPを増やす処理
	void AddHP(const int add) { m_hp += add; }

	// コンストラクタ
	Enemy_HP(int hp)
		:m_hp(hp)
	{}

private:	
	// 変数
	int m_hp;
};

