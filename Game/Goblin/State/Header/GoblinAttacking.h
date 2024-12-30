#pragma once
#ifndef GOBLIN_ATTACKING
#define GOBLIN_ATTACKING

#include "pch.h"
#include "Interface/IState.h"
class Goblin;

class GoblinAttacking : public IState
{

// �Œ�l **
private:
	static constexpr float STATE_TIME = 3.0f;	// ��Ԃ̎���

	static constexpr float CHARGE_TIME = 1.0f;	// �ҋ@����
	static constexpr float ATTACK_TIME = 2.0f;	// �U������
	static constexpr float RETURN_TIME = 3.0f;	// �߂鎞��

	static constexpr float MAX_MINUS_SIZE = 0.35f;
	static constexpr float MAX_Y_POS = 20.0f;

// ���J�֐� **
public:
	// �R���X�g���N�^
	GoblinAttacking(Goblin* goblin);
	// �f�X�g���N�^
	~GoblinAttacking() override;
	// ����������
	void Initialize() override;
	// ���O�X�V����
	void PreUpdate() override;
	// �X�V����
	void Update(const float& elapsedTime) override;
	// ����X�V����
	void PostUpdate() override;
	// �㏈�����s��
	void Finalize() override;

// �����֐� **
private:
	// �v���C���[�̒T�����s���B
	void SearchPlayer();
	// �A�j���[�V�����̍X�V
	void UpdateAnimation();

	void ChargeAnimation();
	void AttackAnimation();
	void ReturnAnimation();

private:
	Goblin* m_goblin;	// �e
	float m_totalTime;	// �o�ߎ���

	float m_angle;	// �p�x
	DirectX::SimpleMath::Matrix m_rotMatrix;		// �ړ��p�̉�]�s��
	DirectX::SimpleMath::Vector3 m_position;		// �ʒu
	DirectX::SimpleMath::Vector3 m_moveValue;		// �ړ���
	DirectX::SimpleMath::Vector3 m_movePosition;	// �ړ��ʒu
};



#endif	// GOBLIN_ATTACKING