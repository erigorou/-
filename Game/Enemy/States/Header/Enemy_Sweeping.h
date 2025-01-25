#pragma once
#ifndef ENEMY_SWEEPING_DEFINED
#define ENEMY_SWEEPING_DEFINED
#include "Interface/IState.h"

class Enemy;
class Player;
class Easing;

// �G�̓ガ�����X�e�[�g���`
class Enemy_Sweeping : public IState
{
public:

	static constexpr float CHARGE_TIME	= 0.5f;		// �U���̒��߃��[�V�����̎���
	static constexpr float WINDUP_TIME	= 0.6f;		// �U������O�̎���
	static constexpr float ATTACK_TIME	= 1.2f;		// �U�����鎞��
	static constexpr float END_TIME		= 2.0f;		// �U���i�S�́j���I�����鎞��
	static constexpr float ROTATE_ANGLE	= 20.0f;	// �U���̒��߃��[�V�����̊p�x

	// �R���X�g���N�^
	Enemy_Sweeping(Enemy* enemy);
	// �f�X�g���N�^
	~Enemy_Sweeping() override;
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

	// �v���C���[�̃{�f�B�Ƃ̓����蔻��
	void CheckHitPlayerBody();

private:
	float m_angle;				// �p�x	
	float m_totalSeconds;		// ������
	Enemy* m_enemy;				// �G�i�X�e�[�g�̌��j	

};

#endif		// ENEMY_SWEEPING_DEFINED
