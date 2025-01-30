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
	// ���b��
	static constexpr float TOTAL_TIME = 2.0f;
	// �ŏ��l
	static constexpr float MINIMAL = 0.01f;
	// ���x�̐��K���p
	static constexpr float NORMALIZE_VELOCITY = 75.0f;
	// �T�C���g�̐U��
	static constexpr float AMPLITUDE = 1.0f;
	// �T�C���g�̎��g��
	static constexpr float FREQUENCY = 1.0f;
	// �J�����̗h��̋���
	static constexpr float SHAKE_POWER = 1.0f;
	// �v���C���[�Ƃ̍ŏ�����
	static constexpr float MIN_DISTANCE = 20.0f;


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

};

#endif		// ENEMY_IDLING_DEFINED
