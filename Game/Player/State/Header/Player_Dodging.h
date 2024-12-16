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
#include "Interface/IPlayer.h"

class Player;
class Enemy;

class PlayerDodging : public IPlayer
{
// �萔
private:
	
	static constexpr float SPIN_TIME = 1.0f;			// ��𒆂̉�]����
	static constexpr float SPIN_REST_TIME = 1.5f;		// �����̉�]����

	static constexpr float ANIMATION_TIME = 0.4f;				// ��𒆂̃A�j���[�V��������
	static constexpr float DODGING_RECOVERY_TIME = 1.0f;	// �����̍d������
	static constexpr float ANIMATION_END_TIME = 1.5f;			// ���[�����O�I������

	static constexpr float DODGE_FUNCTION	= 0.93f;	// ������̖��C
	static constexpr int   TRUNCATION_DIGIT = 4;		// �����Z�Z���ȉ��폜
	static constexpr float DODGING_SPEED = 1.5f;		// ������̑��x
	static constexpr float UP_VALUE = 3.0f;				// �㏸��
	static constexpr float ROLLING_ROT = 360.0f;		// ��]��
	static constexpr float ROLLING_REST_ROT = 30.0f;	// ���[�����O��̉�]�p�x

// ���J�֐�
public:
	// �f�X�g���N�^
	~PlayerDodging() override;
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

	// �R���X�g���N�^
	PlayerDodging(Player* player);

	// �L�[����
	void OnKeyPressed(const DirectX::Keyboard::Keys& key) override;
	// �L�[����
	void OnKeyDown(const DirectX::Keyboard::Keys& key) override;

// �����֐�
private:
	// �v���C���[�̃A�j���[�V�����p�X�V����
	void UpdateAnimation(float totalTime);
	void RollingAnimation(float totalTime);
	void AdjustCharacterTransition(float totalTime);
	// �v���C���[�̈ړ���K�p
	void ApplyPlayerMovement(DirectX::SimpleMath::Vector3& parentPos);


// �����ϐ�
private:
	// ������
	float m_totalSeconds;
	// �I������
	float m_finishTime;

	// �|�W�V����
	DirectX::SimpleMath::Vector3 m_position;
	// �v���C���[�̈ړ����x
	DirectX::SimpleMath::Vector3 m_velocity;
	//
	
	// �v���C���[
	Player* m_player;
	// ���f��
	DirectX::Model* m_model;


	float m_upValue;	// �㏸��
	float m_downValue;	// ���~��

	DirectX::SimpleMath::Vector3 m_rollingValue;
};

#endif  // PLAYER_DODGING_DEFINED