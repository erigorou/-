#pragma once
#ifndef ENEMY_APPROACHING_DEFINED
#define ENEMY_APPROACHIGN_DEFINED
#include "Interface/IState.h"

class Enemy;

// �G�̑ҋ@�X�e�[�g���`
class EnemyApproaching : public IState
{
public:
	// �̂̋��E�����擾
	DirectX::BoundingSphere GetBoundingSphereBody() { return m_boundingSphereBody; }
public:
	// �R���X�g���N�^
	EnemyApproaching(Enemy* enemy);
	// �f�X�g���N�^
	~EnemyApproaching() override;
	// ����������
	void Initialize(DirectX::Model* model) override;
	// ���O�X�V����
	void PreUpdate() override;
	// �X�V��������
	void Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos) override;
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
	Enemy* m_enemy;
	// ���f��
	DirectX::Model* m_model;
	// �̂̋��E��
	DirectX::BoundingSphere m_boundingSphereBody;


	// �T�C���g�̐U��
	float m_amplitude;
	// �T�C���g�̎��g��
	float m_frequency;

};

#endif		// ENEMY_IDLING_DEFINED
