#pragma once
#ifndef ENEMY_IDLING_DEFINED
#define ENEMY_IDLING_DEFINED
#include "Interface/IState.h"

class Boss;
class Player;

// �G�̑ҋ@�X�e�[�g���`
class BossIdling : public IState
{
// ---------------------------
// �Œ�l
// ---------------------------
public:
	// ������
	static constexpr float TOTAL_TIME = 1.0f;
	// ����
	static constexpr float FAR_DISTANCE = 20.0f;
	// �S�̂̊���
	static constexpr int TOTAL_RATE = 10;
	// �ガ�������s������
	static constexpr int SWEEPING_RATE = 3;
	// �����������s������
	static constexpr int ATTACKING_RATE = 7;
	// �_�b�V���U�����s������
	static constexpr int DASH_ATTACK_RATE = 8;
	// �������Ȃ�����
	static constexpr int IDLING_RATE = 10;


// ---------------------------
// �����o�֐��i���J�j
// ---------------------------
public:
	// �R���X�g���N�^
	BossIdling(Boss* boss);
	// �f�X�g���N�^
	~BossIdling() override;
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
// �����o�֐�(����J)
// ---------------------------
private:
	// �A�j���[�V�����̍X�V
	void UpdateAnimation();
	// ���̃X�e�[�g�Ɉڍs���邩�����m
	void CheckNextState();

// ---------------------------
// �����o�ϐ�
// ---------------------------
private:
	// ��]
	float m_angle;

	// ������
	float m_totalSeconds;
	// �G
	Boss* m_boss;
};

#endif		// ENEMY_IDLING_DEFINED
