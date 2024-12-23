#pragma once
#ifndef ENEMY_IDLING_DEFINED
#define ENEMY_IDLING_DEFINED
#include "Interface/IState.h"

class Enemy;
class Player;

// �G�̑ҋ@�X�e�[�g���`
class EnemyIdling : public IState
{
public:
	// �R���X�g���N�^
	EnemyIdling(Enemy* enemy);
	// �f�X�g���N�^
	~EnemyIdling() override;
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

	// �v���C���[�̃{�f�B�Ƃ̓����蔻��
	void CheckHitPlayerBody();



private:
	// ��]
	float m_angle;

	// ������
	float m_totalSeconds;
	// �G
	Enemy* m_enemy;
};

#endif		// ENEMY_IDLING_DEFINED
