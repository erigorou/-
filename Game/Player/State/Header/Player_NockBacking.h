#pragma once

#ifndef PLAYER_NOCKBACKING_DEFINED
#define PLAYER_NOCKBACKING_DEFINED
// -----------------------------
//
// �v���C���[�̂�����
//
// -----------------------------


#pragma once
#include "pch.h"
#include "Interface/IPlayer.h"

class Player;
class Enemy;

class PlayerNockBacking : public IPlayer
{

// �Œ�l *
private:

	static constexpr float NOCKBACKING_TIME = 1.0f;		// ����S�̂̎���
	static constexpr float RETURN_TIME		= 2.0f;		// ���ꂩ�畜�A���鎞��
	static constexpr float END_TIME			= 2.3f;		// ���ꂩ�畜�A���鎞��

	static constexpr float MAX_UP_VALUE		= 2.0f;	// �㏸�l
	static constexpr float RETURN_UP_VALUE	= 1.0f;	// �㏸�l
	static constexpr float SPEED = 50.0f;	// ���x


// ���J�֐� *
public:
	// �f�X�g���N�^
	~PlayerNockBacking() override;
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
	PlayerNockBacking(Player* player);

	// �L�[����
	void OnKeyPressed(const DirectX::Keyboard::Keys& key) override;
	// �L�[����
	void OnKeyDown(const DirectX::Keyboard::Keys& key) override;


// �����֐� *
private:
	void UpdateAnimation();

	void NockBackAnimation();

	void ReturnAnimation();


// �����ϐ� *
private:

	// �o�ߎ���
	float m_elapsedTime;
	// ������
	float m_totalSeconds;
	// �m�b�N�o�b�N����
	float m_nockBackAngle;
	// �X��
	DirectX::SimpleMath::Vector3 m_bodyTilt;

	// �|�W�V����
	DirectX::SimpleMath::Vector3 m_position;
	// �v���C���[�̈ړ����x
	DirectX::SimpleMath::Vector3 m_velocity;
	// �v���C���[
	Player* m_player;
	// ���f��
	DirectX::Model* m_model;

};

#endif  // PLAYER_DODGING_DEFINED