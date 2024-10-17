#pragma once
#ifndef PLAYER_ATTACKING_4_DEFINED
#define PLAYER_ATTACKING_4_DEFINED
// -----------------------------
//
// �v���C���[�̑ҋ@���
//
// -----------------------------


#pragma once
#include "pch.h"
#include "Interface/IPlayer.h"

class Player;

class PlayerAttacking_4 : public IPlayer
{
public:
	// �R���X�g���N�^
	PlayerAttacking_4(Player* player);
	// �f�X�g���N�^
	~PlayerAttacking_4() override;
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

	// �L�[����
	void OnKeyPressed(const DirectX::Keyboard::Keys& key) override;
	// �L�[����
	void OnKeyDown(const DirectX::Keyboard::Keys& key) override;

private:
	// ���W
	DirectX::SimpleMath::Vector3 m_position;
	// ���x
	DirectX::SimpleMath::Vector3 m_velocity;

	// �P��̍U���̊Ԋu
	float m_attackElapsedTime;

	// ������
	float m_totalSeconds;
	// �v���C���[
	Player* m_player;
	// ���f��
	DirectX::Model* m_model;
};

#endif  // PLAYER_ATTACKING_1_DEFINED