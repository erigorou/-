#pragma once
#ifndef PLAYER_ATTACKING_3_DEFINED
#define PLAYER_ATTACKING_3_DEFINED
// -----------------------------
//
// �v���C���[�̑ҋ@���
//
// -----------------------------


#pragma once
#include "pch.h"
#include "Interface/IState.h"

class Player;

class PlayerAttacking_3 : public IState
{
public:


public:
	// �̂̋��E�����擾
	DirectX::BoundingSphere GetBoundingSphereBody() { return m_boundingSphereBody; }

public:
	// �R���X�g���N�^
	PlayerAttacking_3(Player* player);
	// �f�X�g���N�^
	~PlayerAttacking_3() override;
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


private:
	// ���W
	DirectX::SimpleMath::Vector3 m_position;
	// ���x
	DirectX::SimpleMath::Vector3 m_velocity;

	// �P��̍U���̊Ԋu
	unsigned int m_attackElapsedTime;

	// ������
	float m_totalSeconds;
	// �v���C���[
	Player* m_player;
	// ���f��
	DirectX::Model* m_model;
	// �̂̋��E��
	DirectX::BoundingSphere m_boundingSphereBody;

	// �L�[�{�[�h�̓���
	DirectX::Keyboard::State m_keyboardState;
};

#endif  // PLAYER_ATTACKING_1_DEFINED