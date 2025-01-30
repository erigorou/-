#pragma once
#ifndef BossATTACKING_DEFINED
#define BossATTACKING_DEFINED
#include "Interface/IState.h"

class Boss;
class Player;

// �G�̑ҋ@�X�e�[�g���`
class BossAttacking : public IState
{
// ---------------------------
// �Œ�l
// ---------------------------
public:
	// ������
	static constexpr float TOTAL_TIME = 5.0f;
	// �G��ǂ������鎞��
	static constexpr float CHASE_TIME = 1.4f;


// ---------------------------
// �����o�֐��i���J�j
// ---------------------------
public:
	// �R���X�g���N�^
	BossAttacking(Boss* boss);
	// �f�X�g���N�^
	~BossAttacking() override;
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
	// ��]
	float m_angle;
	// ������
	float m_totalSeconds;
	// �G
	Boss* m_boss;
};

#endif		// BossATTACKING_DEFINED
