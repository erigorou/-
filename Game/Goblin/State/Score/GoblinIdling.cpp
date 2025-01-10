#include "pch.h"
#include "../Header/GoblinIdling.h"
#include "../../Goblin.h"
#include "Libraries/MyLib/Math.h"


// -------------------
// �R���X�g���N�^
// -------------------
GoblinIdling::GoblinIdling(Goblin* goblin)
	: m_goblin(goblin)
	, m_TotalTime(0.0f)
{
}


// -------------------
// �f�X�g���N�^
// -------------------
GoblinIdling::~GoblinIdling()
{
}


// -------------------
// ����������
// -------------------
void GoblinIdling::Initialize()
{
	// �����_���ɃN�[���^�C����ݒ肷��
	m_coolTime = Math::RandomFloat(MIN_COOLTIME, MAX_COOLTIME);
}



// -------------------
// ������(in)
// -------------------
void GoblinIdling::PreUpdate()
{
	m_goblin->SetIsAttacking(false);
	m_TotalTime = 0.0f;

	// �����_���ɃN�[���^�C����ݒ肷��
	m_coolTime = Math::RandomFloat(MIN_COOLTIME, MAX_COOLTIME);

}



// -------------------
// �X�V����
// -------------------
void GoblinIdling::Update(const float& elapsedTime)
{
	// ���Ԃ����Z����
	m_TotalTime += elapsedTime;

	// �U���X�e�[�g�ɑJ�ڂ���
	if (m_TotalTime > m_coolTime)
	{
		m_goblin->ChangeState(m_goblin->GetAttacking());
	}
}


// -------------------
// ������(out)
// -------------------
void GoblinIdling::PostUpdate()
{
}


// -------------------
// �㏈�����s��
// -------------------
void GoblinIdling::Finalize()
{
}