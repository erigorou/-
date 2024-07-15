#pragma once
#ifndef ENEMY_ATTACKING_DEFINED
#define ENEMY_ATTACKING_DEFINED
#include "Interface/IState.h"

class Enemy;
class Player;

// �G�̑ҋ@�X�e�[�g���`
class Enemy_Attacking : public IState
{
public:
	// �̂̋��E�����擾
	DirectX::BoundingSphere GetBoundingSphereBody() { return m_boundingSphereBody; }
public:
	// �R���X�g���N�^
	Enemy_Attacking(Enemy* enemy);
	// �f�X�g���N�^
	~Enemy_Attacking() override;
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
	// ��]
	float m_angle;

	// ������
	float m_totalSeconds;
	// �G
	Enemy* m_enemy;
	// ���f��
	DirectX::Model* m_model;
	// �̂̋��E��
	DirectX::BoundingSphere m_boundingSphereBody;
};

#endif		// ENEMY_IDLING_DEFINED
