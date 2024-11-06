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
// �Œ�l
public:
	static constexpr float DOWN_TIME = 0.4f;

// ���J�֐�
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

	// �L�[����
	void OnKeyPressed(const DirectX::Keyboard::Keys& key) override;
	// �L�[����
	void OnKeyDown(const DirectX::Keyboard::Keys& key) override;


// �����֐�
private:
	void UpdateAnimation();


// �����ϐ�
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

	// �A�j���[�V�����p ///
	float m_upValue;	// �㏸�l
	float m_downValue;	// ���~�l
};

#endif  // PLAYER_ATTACKING_1_DEFINED