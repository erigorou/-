#pragma once
#ifndef ENEMY_STARTING_DEFINED
#define ENEMY_STARTING_DEFINED
#include "Interface/IState.h"

class Enemy;
class Player;

// �G�̑ҋ@�X�e�[�g���`
class EnemyStarting : public IState
{
public:
	static constexpr float TOTAL_TIME = 4.0f;
	static constexpr float DELAY_TIME = 0.5f;
	static constexpr float MOVE_TIME  = 2.5f;
	static constexpr float START_HEIGHT = 100.0f;
	static constexpr float PLAY_EFFECT_HEIGHT = 5.0f;

public:
	// �R���X�g���N�^
	EnemyStarting(Enemy* enemy);
	// �f�X�g���N�^
	~EnemyStarting() override;
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
	void UpdateAnimation();
	void UpdateDelay();
	void UpdateMove();
	void PlayEffect();

private:
	// �ʒu
	DirectX::SimpleMath::Vector3 m_position;
	// ��]
	float m_angle;

	bool m_isEndDelay;

	// ������
	float m_totalSeconds;
	// �G
	Enemy* m_enemy;
};

#endif		// ENEMY_IDLING_DEFINED
