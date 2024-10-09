#pragma once

#ifndef ENEMY_HP
#define ENEMY_HP
#include "Interface/IHitPoint.h"

#include "pch.h"

class EnemyHP
{
private:
    static constexpr int MAX_HP = 20; // 最大HP


public:
    EnemyHP() : m_hp(MAX_HP) {}    // コンストラクタ
    ~EnemyHP() = default;          // デストラクタ


    int  GetMaxHP() const { return MAX_HP; }  // 最大HPの取得
    int  GetHP() const { return m_hp; }    // 現在のHPの取得
    void SetHP(int hp) { m_hp = hp; }      // 現在のHPの設定


    // 回復
    void Heal(int hp) { m_hp = std::min(m_hp + hp, MAX_HP); }

    // ダメージ
    bool Damage(int hp) {
        m_hp = std::max(m_hp - hp, 0);
        return CheckPlayerDie();
    }

private:
    // プレイヤーが死んでいるかどうか
    bool CheckPlayerDie() const { return m_hp <= 0; }


    int m_hp;     // 現在のHP
};

#endif

