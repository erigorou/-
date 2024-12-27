#include "pch.h"
#include "../Header/GoblinAttacking.h"
#include "../../Goblin.h"

// �R���X�g���N�^
GoblinAttacking::GoblinAttacking(Goblin* goblin)
	: m_goblin(goblin)
	, m_totalTime(0.0f)
{
}

// �I������
GoblinAttacking::~GoblinAttacking()
{
}


// ����������
void GoblinAttacking::Initialize()
{
}


// ���O�X�V
void GoblinAttacking::PreUpdate()
{
	m_goblin->SetIsAttacking(true);
}


// �X�V
void GoblinAttacking::Update(const float& elapsedTime)
{
	// ���Ԃ����Z����
	m_totalTime += elapsedTime;
}


// ����X�V
void GoblinAttacking::PostUpdate()
{
	m_goblin->SetIsAttacking(false);
}

// �I������
void GoblinAttacking::Finalize()
{
}

// �v���C���[�̒T�����s���B
void GoblinAttacking::SearchPlayer()
{

}


