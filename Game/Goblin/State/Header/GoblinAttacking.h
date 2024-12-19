#pragma once
#ifndef GOBLIN_ATTACKING
#define GOBLIN_ATTACKING

#include "pch.h"
#include "Interface/IState.h"

#include "../../Goblin.h"

class Goblin::GoblinAttacking : public IState
{
public:
	// �R���X�g���N�^
	GoblinAttacking(Goblin* goblin);
	// �f�X�g���N�^
	~GoblinAttacking() override;
	// ����������
	void Initialize(DirectX::Model* model) override;
	// ���O�X�V����
	void PreUpdate() override;
	// �X�V����
	void Update(const float& elapsedTime) override;
	// ����X�V����
	void PostUpdate() override;
	// �`�悷��
	void Render(
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection) override;

	// �㏈�����s��
	void Finalize() override;

private:
	Goblin* m_goblin;	// �e

	DirectX::Model* m_model;
	float m_totalTime;
};



#endif	// GOBLIN_ATTACKING