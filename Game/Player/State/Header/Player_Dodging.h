#pragma once
#ifndef PLAYER_DODGING_DEFINED
#define PLAYER_DODGING_DEFINED
// -----------------------------
//
// �v���C���[�̑ҋ@���
//
// -----------------------------


#pragma once
#include "pch.h"
#include "Interface/IState.h"

class Player;
class Enemy;

class PlayerDodging : public IState
{

private:
	/////////////////////////�Œ�l///////////////////////////////
	static const float DODGING_TIME;		// ���S�̂̎���
	static const float DODGE_FUNCTION;		// ������̖��C
	static const int   TRUNCATION_DIGIT;	// �����Z�Z���ȉ��폜

	static constexpr float DODGING_SPEED = 3.0f;	// ������̑��x

public:
	// �f�X�g���N�^
	~PlayerDodging() override;
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

	// �R���X�g���N�^
	PlayerDodging(Player* player);

private:
	// ������
	float m_totalSeconds;
	// �I������
	float m_finishTime;

	// ���������擾���邽�߂̌���
	DirectX::SimpleMath::Vector3 m_direction;
	// �v���C���[�̈ړ����x
	DirectX::SimpleMath::Vector3 m_velocity;
	// �v���C���[
	Player* m_player;
	// ���f��
	DirectX::Model* m_model;

};

#endif  // PLAYER_DODGING_DEFINED