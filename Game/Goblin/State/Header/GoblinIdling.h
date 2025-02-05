#pragma once
#ifndef GOBLIN_IDLING
#define GOBLIN_IDLING

#include "pch.h"
#include "Interface/IState.h"
class Goblin;

class GoblinIdling : public IState
{
	// -----------0-
	// �Œ�l
	// -------------
public:
	static constexpr float MIN_COOLTIME = 1.0f;
	static constexpr float MAX_COOLTIME = 3.0f;

	// -----------0-
	// ���J�֐�
	// -------------
public:
	// �R���X�g���N�^
	GoblinIdling(Goblin* goblin);
	// �f�X�g���N�^
	~GoblinIdling() override;
	// ����������
	void Initialize() override;
	// ���O�X�V����
	void PreUpdate() override;
	// �X�V����
	void Update(const float& elapsedTime) override;
	// ����X�V����
	void PostUpdate() override;
	// �㏈�����s��
	void Finalize() override;
	// �v���C���[��T��
	void SearchPlayer();


private:
	Goblin* m_goblin;	// �e
	float m_TotalTime;	// �o�ߎ���

	float m_coolTime;	// �N�[���^�C��

	// �ʒu
	DirectX::SimpleMath::Vector3 m_position;
	// �p�x
	float m_angle;
	// ��]�s��
	DirectX::SimpleMath::Matrix m_rotMatrix;
};

#endif