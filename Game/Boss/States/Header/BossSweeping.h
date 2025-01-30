#pragma once
#ifndef BOSS_SWEEPING_DEFINED
#define BOSS_SWEEPING_DEFINED
#include "Interface/IState.h"

class Boss;
class Player;
class Easing;

// �G�̓ガ�����X�e�[�g���`
class BossSweeping : public IState
{
	// ---------------------------
	// �Œ�l
	// ---------------------------
public:
	// �U���̒��߃��[�V�����̎���
	static constexpr float CHARGE_TIME = 0.5f;
	// �U������O�̎���
	static constexpr float WINDUP_TIME = 0.6f;
	// �U�����鎞��
	static constexpr float ATTACK_TIME = 1.2f;
	// �U���i�S�́j���I�����鎞��
	static constexpr float END_TIME = 2.0f;
	// �U���̒��߃��[�V�����̊p�x
	static constexpr float ROTATE_ANGLE = 20.0f;


// ---------------------------
// �����o�֐��i���J�j
// ---------------------------
public:
	// �R���X�g���N�^
	BossSweeping(Boss* boss);
	// �f�X�g���N�^
	~BossSweeping() override;
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
// �����o�ϐ�
// ---------------------------
private:
	// �p�x
	float m_angle;
	// ������
	float m_totalSeconds;
	// �G
	Boss* m_boss;

};

#endif		// BossSweeping_DEFINED
