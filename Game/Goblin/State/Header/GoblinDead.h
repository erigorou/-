#pragma once
#ifndef GOBLIN_DEAD
#define GOBLIN_DEAD

#include "pch.h"
#include "Interface/IState.h"
class Goblin;

class GoblinDead : public IState
{

// -----------0-
// �Œ�l
// -------------
public:
	static constexpr float DEAD_TIME = 2.0f;



// -----------0-
// ���J�֐�
// -------------
public:

	GoblinDead(Goblin* goblin);

	~GoblinDead() override;

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


// -------------------
// �����֐�
// -------------------
private:	
	void UpdateAnimation();



// �����o
private:
	Goblin* m_goblin;	// �e
	float m_totalTime;	// �o�ߎ���
	float m_scale;

	DirectX::SimpleMath::Vector3	m_startPos;		// �J�n�n�_
	float							m_startPos_y;	// �J�n�n�__Y

};


#endif