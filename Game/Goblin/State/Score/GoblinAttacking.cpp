#include "pch.h"
#include "../Header/GoblinAttacking.h"

// �R���X�g���N�^
Goblin::GoblinAttacking::GoblinAttacking(Goblin* goblin)
	: m_goblin(goblin)
	, m_totalTime(0.0f)
{
}

// �I������
Goblin::GoblinAttacking::~GoblinAttacking()
{
}


// ����������
void Goblin::GoblinAttacking::Initialize()
{
}


// ���O�X�V
void Goblin::GoblinAttacking::PreUpdate()
{
	m_goblin->SetIsAttacking(true);
}


// �X�V
void Goblin::GoblinAttacking::Update(const float& elapsedTime)
{
	// ���Ԃ����Z����
	m_totalTime += elapsedTime;
}


// ����X�V
void Goblin::GoblinAttacking::PostUpdate()
{
	m_goblin->SetIsAttacking(false);
}

// �I������
void Goblin::GoblinAttacking::Finalize()
{
}

// �v���C���[�̒T�����s���B
void Goblin::GoblinAttacking::SearchPlayer()
{

}


