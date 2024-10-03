#pragma once

#ifndef PLAYER_HP
#define PLAYER_HP

#include "pch.h"

class PlayerHP
{
public:


private:
	const int MAX_HP = 5; // �ő�HP

public:
    PlayerHP() : m_hp(MAX_HP) {}    // �R���X�g���N�^
    ~PlayerHP() = default;          // �f�X�g���N�^


	int  GetMaxHP   () const    { return MAX_HP; } // �ő�HP�̎擾
	int  GetHP      () const    { return m_hp; }    // ���݂�HP�̎擾
	void SetHP      (int hp)    { m_hp = hp; }      // ���݂�HP�̐ݒ�


    // HP�̑���
    void PlusPlayerHP(int hp)   { m_hp = std::min(m_hp + hp, MAX_HP); }
    bool MinusPlayerHP(int hp)  { 
        m_hp -= hp; 
        return m_hp <= 0;       }


    // �v���C���[������ł��邩�ǂ���
    bool CheckPlayerDie() const { return m_hp <= 0; }

private:
    int m_hp;     // ���݂�HP
};

#endif

