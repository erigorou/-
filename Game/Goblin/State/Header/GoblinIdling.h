#pragma once
#ifndef GOBLIN_IDLING
#define GOBLIN_IDLING

#include "pch.h"
#include "../../Goblin.h"


class Goblin::GoblinIdling : public IState
{

// ����
public:

	GoblinIdling(Goblin* goblin);

	~GoblinIdling() override;

	// ����������
	void Initialize(DirectX::Model* model) override;

	// ���O�X�V����
	void PreUpdate() override;

	// �X�V����
	void Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos) override;

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


// �����o
private:





	// �Œ�l



	Goblin* m_goblin;	// �e
};


#endif