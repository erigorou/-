#pragma once
#ifndef IPLAYER
#define IPLAYER

#include "pch.h"
#include "Interface/IState.h"

class IPlayer : public IState
{
public:
	// �f�X�g���N�^
	virtual ~IPlayer() = default;
	// ����������
	virtual void Initialize(DirectX::Model* model) = 0;
	// ���O�X�V����
	virtual void PreUpdate() = 0;
	// �X�V����
	virtual void Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos) = 0;
	// �L�[���͂�ʒm
	virtual void OnKeyPressed(const DirectX::Keyboard::Keys& key) = 0;
	// ����X�V����
	virtual void PostUpdate() = 0;
	// �`�悷��
	virtual void Render(
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection) = 0;
	// �㏈�����s��
	virtual void Finalize() = 0;
};

#endif // !1

