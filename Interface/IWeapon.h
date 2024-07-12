#pragma once
#include "pch.h"


class IWeapon
{
public:
	// �f�X�g���N�^
	virtual~IWeapon() = default;
	// ������
	virtual void Initialize() = 0;
	// ���O����
	virtual void PreUpdate() = 0;
	// �X�V����
	virtual void Update(float elapsedTime) = 0;
	// ���㏈��
	virtual void PostUpdate() = 0;
	// �`�揈��
	virtual void Render(
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection) = 0;
	// �I������
	virtual void Finalize() = 0;
	// ����̓����蔻����擾����֐�
	virtual DirectX::BoundingBox GetBoundingBox() = 0;
};