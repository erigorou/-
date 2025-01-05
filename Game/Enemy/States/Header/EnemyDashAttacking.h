
#pragma once
#ifndef ENEMY_DASH_ATTACKING_DEFINED
#define ENEMY_DASH_ATTACKING_DEFINED
#include "pch.h"
#include "Interface/IState.h"


class Enemy;
class Player;

// �G�̑ҋ@�X�e�[�g���`
class EnemyDashAttacking : public IState
{
// �Œ�l *
public:
	static constexpr float CHARGE_TIME	= 1.0f;	// �U���̒��߃��[�V�����̎���
	static constexpr float DASH_TIME	= 2.0f;	// �_�b�V�����鎞��
	static constexpr float WAIT_TIME	= 3.5f;	// �ҋ@
	static constexpr float RETURN_TIME	= 5.0f;	// �U�����I����Ă��猳�ɖ߂鎞��
	static constexpr float TOTAL_TIME	= 5.0f;	// ������

	static constexpr float MAX_SPEED = 100.0f; // �ő呬�x


// ���J�֐� *
public:
	// �R���X�g���N�^
	EnemyDashAttacking(Enemy* enemy);
	// �f�X�g���N�^
	~EnemyDashAttacking() override;
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

// �����֐��@*
private:
	void UpdateAction();

	////// �s���n

	// �U���̒��߃��[�V����
	void ChargeAction();
	// �_�b�V������
	void DashAction();
	// �ҋ@
	void WaitAction();
	// ���ɖ߂�
	void ReturnAction();


// �����ϐ��@*
private:
	// ��]
	float m_angle;
	// �X��
	float m_bodyTilt;
	// �U����
	bool m_isAttacking;

	DirectX::SimpleMath::Matrix m_rotMatrix;
	DirectX::SimpleMath::Vector3 m_velocity;

	// �t���[������
	float m_elapsedTime;
	// ������
	float m_totalSeconds;
	// �G
	Enemy* m_enemy;
};

#endif		// ENEMY_DASH_ATTACKING_DEFINED
