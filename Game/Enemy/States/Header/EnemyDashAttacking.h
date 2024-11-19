
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
	static constexpr float DASH_TIME	= 3.0f;	// �_�b�V�����鎞��
	static constexpr float WAIT_TIME	= 4.0f;	// �ҋ@
	static constexpr float RETURN_TIME	= 4.5f;	// �U�����I����Ă��猳�ɖ߂鎞��
	static constexpr float TOTAL_TIME	= 5.0f;	// ������

	static constexpr float MAX_SPEED = 100.0f; // �ő呬�x


// ���J�֐� *
public:
	// �R���X�g���N�^
	EnemyDashAttacking(Enemy* enemy);
	// �f�X�g���N�^
	~EnemyDashAttacking() override;
	// ����������
	void Initialize(DirectX::Model* model) override;
	// ���O�X�V����
	void PreUpdate() override;
	// �X�V��������
	void Update(const float& elapsedTime) override;
	// ����X�V����
	void PostUpdate() override;
	// �`�悷��
	void Render(
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection) override;
	// �I������
	void Finalize() override;

// �����֐��@*
private:
	void UpdateAction(float elapsedTime);

	////// �s���n

	// �U���̒��߃��[�V����
	void ChargeAction();
	// �_�b�V������
	void DashAction(float elapsedTime);
	// �ҋ@
	void WaitAction();
	// ���ɖ߂�
	void ReturnAction();


// �����ϐ��@*
private:
	// ��]
	float m_angle;
	// ������
	float m_totalSeconds;
	// �G
	Enemy* m_enemy;
	// ���f��
	DirectX::Model* m_model;

};

#endif		// ENEMY_DASH_ATTACKING_DEFINED
