#pragma once

#ifndef PLAYER_HP
#define PLAYER_HP
#include "Interface/IHitPoint.h"

#include "pch.h"

class PlayerHP
{
private:
	static constexpr int MAX_HP = 5; // �ő�HP


public:
    PlayerHP() : m_hp(MAX_HP) {}    // �R���X�g���N�^
    ~PlayerHP() = default;          // �f�X�g���N�^


	int  GetMaxHP   () const { return MAX_HP; }  // �ő�HP�̎擾
	int  GetHP      () const { return m_hp; }    // ���݂�HP�̎擾
	void SetHP      (int hp) { m_hp = hp; }      // ���݂�HP�̐ݒ�


    // ��
    void Heal(int hp) { m_hp = std::min(m_hp + hp, MAX_HP); }
    
    // �_���[�W
    bool Damage(int hp){
		m_hp = std::max(m_hp - hp, 0);
        return CheckPlayerDie();
    }

private:
    // �v���C���[������ł��邩�ǂ���
    bool CheckPlayerDie() const { return m_hp <= 0; }


    int m_hp;     // ���݂�HP
};

#endif

