#pragma once
#ifndef GOBLIN_HP
#define GOBLIN_HP
#include "Interface/IHitPoint.h"

#include "pch.h"

class GoblinHP
{
// �Œ�l
private:
	static constexpr int MAX_HP = 3; // �ő�HP


// ���ʊ֐�
public:
	GoblinHP() : m_hp(MAX_HP) {}    // �R���X�g���N�^
	~GoblinHP() = default;          // �f�X�g���N�^

	int GetMaxHP() const { return MAX_HP; }  // �ő�HP�̎擾
	int GetHP() const { return m_hp; }    // ���݂�HP�̎擾
	void SetHP(int hp) { m_hp = hp; }      // ���݂�HP�̐ݒ�

	// ��
	void Heal(int hp)	
	{
		m_hp = std::min(m_hp + hp, MAX_HP); 
	}

	// �_���[�W
	bool Damage(int hp) 
	{
		m_hp = std::max(m_hp - hp, 0);
		return CheckGoblinDie();
	}


// �����o�֐�
private:
	// �S�u����������ł��邩�ǂ���
	bool CheckGoblinDie() const { return m_hp <= 0; }



// �����o�ϐ�
private:
	int m_hp;
};


#endif // !GOBLIN_HP