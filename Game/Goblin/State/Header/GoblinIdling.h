#pragma once
#ifndef GOBLIN_IDLING
#define GOBLIN_IDLING

#include "pch.h"
#include "Interface/IState.h"
class Goblin;

class GoblinIdling : public IState
{

// -----------0-
// �Œ�l
// -------------
public:
	static constexpr float MIN_COOLTIME = 1.0f;
	static constexpr float MAX_COOLTIME = 3.0f;



// -----------0-
// ���J�֐�
// -------------
public:

	GoblinIdling(Goblin* goblin);

	~GoblinIdling() override;

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


// �����o
private:
	Goblin* m_goblin;	// �e
	float m_TotalTime;	// �o�ߎ���

	float m_coolTime;	// �N�[���^�C��

};


#endif