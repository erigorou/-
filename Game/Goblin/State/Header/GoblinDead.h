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
	static constexpr float DEAD_TIME = 2.5f;



// -----------0-
// ���J�֐�
// -------------
public:

	// �R���X�g���N�^
	GoblinDead(Goblin* goblin);

	// �f�X�g���N�^
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

	// �A�j���[�V�����̍X�V����
	void UpdateAnimation();


// -------------------
// �����ϐ�
// -------------------
private:

	Goblin* m_goblin;	// �e
	float m_totalTime;	// �o�ߎ���
	float m_scale;		// �X�P�[��
	DirectX::SimpleMath::Vector3 m_startPos;	// �J�n�n�_
};


#endif