#pragma once
#include "pch.h"

#include "Game/Enemy/Enemy.h"

class Enemy_HP
{
public:

	// HP���擾���鏈��
	int  GetHP() const { return m_hp; }
	// HP��ݒ肷�鏈��
	void SetHP(const int set) { m_hp = set; }

	// HP�����炷����
	void ReduceHP(const int reduce) { m_hp -= reduce; }
	// HP�𑝂₷����
	void AddHP(const int add) { m_hp += add; }

	// �R���X�g���N�^
	Enemy_HP(int hp)
		:m_hp(hp)
	{}

private:	
	// �ϐ�
	int m_hp;
};

