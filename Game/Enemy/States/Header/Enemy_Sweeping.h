#pragma once
#ifndef ENEMY_SWEEPING_DEFINED
#define ENEMY_SWEEPING_DEFINED
#include "Interface/IState.h"

class Enemy;
class Player;
class Easying;

// �G�̓ガ�����X�e�[�g���`
class Enemy_Sweeping : public IState
{
public:

	static const float CHARGE_TIME;	// �U���̒��߃��[�V�����̎���
	static const float WINDUP_TIME;	// �U������O�̎���
	static const float ATTACK_TIME;	// �U�����鎞��
	static const float END_TIME;	// �U���i�S�́j���I�����鎞��

	static const float ROTATE_ANGLE;	// �U���̒��߃��[�V�����̊p�x

	// �R���X�g���N�^
	Enemy_Sweeping(Enemy* enemy);
	// �f�X�g���N�^
	~Enemy_Sweeping() override;
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

	// �v���C���[�̃{�f�B�Ƃ̓����蔻��
	void CheckHitPlayerBody();

private:
	float m_angle;				// �p�x	
	float m_totalSeconds;		// ������
	Enemy* m_enemy;				// �G�i�X�e�[�g�̌��j	
	DirectX::Model* m_model;	// ���f��

};

#endif		// ENEMY_SWEEPING_DEFINED
