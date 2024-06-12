// -------------------------------
//
// �X�e�[�g�p�^�[���̊�b
// �v���C���[�ƓG
//
// -------------------------------


#pragma once
#ifndef ISTATE_DEFINED
#define ISTATE_DEFINED


class IState
{
public:
	virtual ~IState() = default;
	// ����������
	virtual void Initialize() = 0;
	// ���O�X�V����
	virtual void PreUpdate() = 0;
	// �X�V����
	virtual void Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos) = 0;
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

#endif