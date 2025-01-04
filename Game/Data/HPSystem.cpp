// ---------------=============================-------------------//
//�@HP�V�X�e��
//  11/11(��)
// ---------------=============================-------------------//

#include "pch.h"
#include "HPSystem.h"


// -------------------
// �R���X�g���N�^
// -------------------
HPSystem::HPSystem(float max)
	: m_maxHP(max)
	, m_hp(max)
{
	
}


// -------------------
// �f�X�g���N�^
// -------------------
HPSystem::~HPSystem()
{
}


// -------------------
// ��
// -------------------
void HPSystem::Heal(float hp)
{
	m_hp = std::min(m_hp + hp, m_maxHP);
}


// -------------------
// �_���[�W
// -------------------
bool HPSystem::Damage(float hp)
{
	m_hp = std::max(m_hp - hp, 0.0f);
	return CheckDie();
}



// -------------------
// ���S����
// -------------------
bool HPSystem::CheckDie()
{
	return m_hp <= 0.0f;
}