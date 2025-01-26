#pragma once
#ifndef ENEMY_APPROACHING_DEFINED
#define ENEMY_APPROACHIGN_DEFINED
#include "Interface/IState.h"

class Enemy;
class Particle;

// �G�̑ҋ@�X�e�[�g���`
class EnemyApproaching : public IState
{
	// ---------------------------
	// �Œ�l
	// ---------------------------
public:
	static constexpr float MINIMAL = 0.01f;				// �ŏ��l
	static constexpr float NORMALIZE_VELOCITY = 75.0f; 	// ���x�̐��K��



	// ---------------------------
	// ���J�֐�
	// ---------------------------
public:
	// �R���X�g���N�^
	EnemyApproaching(Enemy* enemy);
	// �f�X�g���N�^
	~EnemyApproaching() override;
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
	Enemy* m_enemy;
	// �p�[�e�B�N��
	Particle* m_particles;

	// �T�C���g�̐U��
	float m_amplitude;
	// �T�C���g�̎��g��
	float m_frequency;
	// �J�����̗h��̋���
	float m_shakePower;

};

#endif		// ENEMY_IDLING_DEFINED
