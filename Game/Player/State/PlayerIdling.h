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

class PlayerIdling : IState
{
public:
	// �R���X�g���N�^
	PlayerIdling(Player* player);
	// �f�X�g���N�^
	~PlayerIdling() override;
	// ����������
	void Initialize() override;
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

private:
	// ������
	float m_totalSeconds;
	// �I������
	float m_finishTime;
	// �v���C���[
	Player* m_player;
};

#endif  // PLAYER_IDLING_DEFINED