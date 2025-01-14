#pragma once
#ifndef ENEMY_DEAD_DEFINED
#define ENEMY_DEAD_DEFINED
#include "Interface/IState.h"

class Enemy;
class Player;

// �G�̑ҋ@�X�e�[�g���`
class EnemyDead : public IState
{
// ---------------------------
// �Œ�l
// ---------------------------
private:
	static constexpr float TOTAL_TIME = 3.0f;


// ---------------------------
// ���J�֐�
// ---------------------------
public:
	// �R���X�g���N�^
	EnemyDead(Enemy* enemy);
	// �f�X�g���N�^
	~EnemyDead() override;
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


// ---------------------------
// �����֐�
// ---------------------------
private:
	// �A�j���[�V�����̍X�V
	void UpdateAnimation();

// ---------------------------
// �����o�ϐ�
// ---------------------------
private:
	// ��]
	float m_angle;

	// ������
	float m_totalSeconds;
	// �G
	Enemy* m_enemy;

	// �A�j���[�V�����p�̉�]���W
	float m_tiltAngle;
};

#endif		// ENEMY_IDLING_DEFINED
