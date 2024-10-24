#pragma once
#ifndef ENEMY_TITLEMOVING_DEFINED
#define ENEMY_TITLEMOVING_DEFINED
#include "Interface/IState.h"

class Enemy;

// �G�̑ҋ@�X�e�[�g���`
class EnemyTitleMoving : public IState
{
public:

	// �R���X�g���N�^
	EnemyTitleMoving(TitleEnemy* enemy);
	// �f�X�g���N�^
	~EnemyTitleMoving() override;
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



private:
	static constexpr float MINIMAL		= 0.01f;
	static constexpr float COOL_TIME	= 0.5f;

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
	TitleEnemy* m_enemy;
	// ���f��
	DirectX::Model* m_model;


	// �T�C���g�̐U��
	float m_amplitude;
	// �T�C���g�̎��g��
	float m_frequency;
	// �W�����v����
	bool m_isJump;
	// �V�F�C�N�ł��邩
	bool m_canShake;

};

#endif		// ENEMY_IDLING_DEFINED
