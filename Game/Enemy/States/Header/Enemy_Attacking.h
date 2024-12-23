#pragma once
#ifndef ENEMY_ATTACKING_DEFINED
#define ENEMY_ATTACKING_DEFINED
#include "Interface/IState.h"

class Enemy;
class Player;

// �G�̑ҋ@�X�e�[�g���`
class Enemy_Attacking : public IState
{
public:
	// �R���X�g���N�^
	Enemy_Attacking(Enemy* enemy);
	// �f�X�g���N�^
	~Enemy_Attacking() override;
	// ����������
	void Initialize() override;
	// ���O�X�V����
	void PreUpdate() override;
	// �X�V��������
	void Update(const float& elapsedTime) override;
	// ����X�V����
	void PostUpdate() override;
	// �I������
	void Finalize() override;



private:
	// ��]
	float m_angle;

	// ������
	float m_totalSeconds;
	// �G
	Enemy* m_enemy;

};

#endif		// ENEMY_ATTACKING_DEFINED
