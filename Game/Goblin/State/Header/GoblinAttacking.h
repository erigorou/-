#pragma once
#ifndef GOBLIN_ATTACKING
#define GOBLIN_ATTACKING

#include "pch.h"
#include "Interface/IState.h"
class Goblin;

class GoblinAttacking : public IState
{
public:
	// �R���X�g���N�^
	GoblinAttacking(Goblin* goblin);
	// �f�X�g���N�^
	~GoblinAttacking() override;
	// ����������
	void Initialize() override;
	// ���O�X�V����
	void PreUpdate() override;
	// �X�V����
	void Update(const float& elapsedTime) override;
	// ����X�V����
	void PostUpdate() override;
	// �㏈�����s��
	void Finalize() override;

// �����֐� **
private:
	// �v���C���[�̒T�����s���B
	void SearchPlayer();



private:
	Goblin* m_goblin;			// �e
	float m_totalTime;			// �o�ߎ���

	
};



#endif	// GOBLIN_ATTACKING