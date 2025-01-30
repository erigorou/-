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
// ���J�֐�
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
// �����֐�
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
