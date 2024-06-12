#pragma once
#ifndef ENEMY_IDLING_DEFINED
#define ENEMY_IDLING_DEFINED
#include "Interface/IState.h"

class Enemy;

// �G�̑ҋ@�X�e�[�g���`
class EnemyIdling : public IState
{
public:
	// �R���X�g���N�^
	EnemyIdling(Enemy* enemy);
	// �f�X�g���N�^
	~EnemyIdling() override;
	// ����������
	void Initialize() override;
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


private:
	// ������
	float m_totalSeconds;
	// �I������
	float m_finishTime;
	// �G
	Enemy* m_enemy;
};

#endif		// ENEMY_IDLING_DEFINED
