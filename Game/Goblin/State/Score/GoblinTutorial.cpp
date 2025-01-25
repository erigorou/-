#include "pch.h"
#include "../Header/GoblinTutorial.h"
#include "../../Goblin.h"
#include "Libraries/MyLib/Math.h"


// -------------------
// �R���X�g���N�^
// -------------------
GoblinTutorial::GoblinTutorial(Goblin* goblin)
	: m_goblin(goblin)
	, m_TotalTime(0.0f)
{
}


// -------------------
// �f�X�g���N�^
// -------------------
GoblinTutorial::~GoblinTutorial()
{
}


// -------------------
// ����������
// -------------------
void GoblinTutorial::Initialize()
{
	// �����_���ɃN�[���^�C����ݒ肷��
	m_coolTime = Math::RandomFloat(MIN_COOLTIME, MAX_COOLTIME);
}



// -------------------
// ������(in)
// -------------------
void GoblinTutorial::PreUpdate()
{
	m_goblin->SetIsAttacking(false);
	m_TotalTime = 0.0f;

	// �����_���ɃN�[���^�C����ݒ肷��
	m_coolTime = Math::RandomFloat(MIN_COOLTIME, MAX_COOLTIME);

}



// -------------------
// �X�V����
// -------------------
void GoblinTutorial::Update(const float& elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}


// -------------------
// ������(out)
// -------------------
void GoblinTutorial::PostUpdate()
{
}


// -------------------
// �㏈�����s��
// -------------------
void GoblinTutorial::Finalize()
{
}