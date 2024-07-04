#pragma once
#ifndef PLAYER_IDLING_DEFINED
#define PLAYER_IDLING_DEFINED
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

class PlayerIdling : public IState
{
public:
	// �̂̓����蔻����擾
	DirectX::BoundingSphere GetBoundingSphereBody() { return m_boundingSphereBody; };

public:
	// �f�X�g���N�^
	~PlayerIdling() override;
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
	PlayerIdling(Player* player);
	// �̂Ɠ��������Ƃ��̏���
	DirectX::SimpleMath::Vector3 CalculatingPushBack();

private:
	// ������
	float m_totalSeconds;
	// �v���C���[
	Player* m_player;
	// ���f��
	DirectX::Model* m_model;
	// �̂̓����蔻��
	DirectX::BoundingSphere m_boundingSphereBody;

	// �L�[�{�[�h�̓���
	DirectX::Keyboard::State m_keyboardState;
};

#endif  // PLAYER_IDLING_DEFINED