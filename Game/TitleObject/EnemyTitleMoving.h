#pragma once
#ifndef ENEMY_TITLEMOVING_DEFINED
#define ENEMY_TITLEMOVING_DEFINED
#include "Interface/IState.h"

class Enemy;
class Particle;

// �G�̑ҋ@�X�e�[�g���`
class EnemyTitleMoving : public IState
{


public:

	// �R���X�g���N�^
	EnemyTitleMoving(TitleEnemy* enemy);
	// �f�X�g���N�^
	~EnemyTitleMoving() override;
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
	static constexpr float MINIMAL		= 0.01f;
	static constexpr float COOL_TIME	= 0.5f;

	// ���W
	DirectX::SimpleMath::Vector3 m_position;
	// �ړ����x
	DirectX::SimpleMath::Vector3 m_velocity;
	// ��]
	float m_angle;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_worldMat;

	// ������
	float m_totalSeconds;
	// �I������
	float m_finishTime;
	// �G
	TitleEnemy* m_enemy;


	// �T�C���g�̐U��
	float m_amplitude;
	// �T�C���g�̎��g��
	float m_frequency;
	// �W�����v����
	bool m_isJump;
	// �V�F�C�N�ł��邩
	bool m_canShake;
	// �V�F�C�N�p���[
	float m_shakePower;

};

#endif		// ENEMY_IDLING_DEFINED
