#pragma once

#ifndef PLAYER_HP
#define PLAYER_HP

#include "pch.h"

class PlayerHP
{
public:


private:
	const int MAX_HP = 5; // 最大HP

public:
    PlayerHP() : m_hp(MAX_HP) {}    // コンストラクタ
    ~PlayerHP() = default;          // デストラクタ


	int  GetMaxHP   () const    { return MAX_HP; } // 最大HPの取得
	int  GetHP      () const    { return m_hp; }    // 現在のHPの取得
	void SetHP      (int hp)    { m_hp = hp; }      // 現在のHPの設定


    // HPの増減
    void PlusPlayerHP(int hp)   { m_hp = std::min(m_hp + hp, MAX_HP); }
    bool MinusPlayerHP(int hp)  { 
        m_hp -= hp; 
        return m_hp <= 0;       }


    // プレイヤーが死んでいるかどうか
    bool CheckPlayerDie() const { return m_hp <= 0; }

private:
    int m_hp;     // 現在のHP
};

#endif

