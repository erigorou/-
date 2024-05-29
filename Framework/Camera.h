#pragma once
#ifndef CAMERA_DEFINED
#define CAMERA_DEFINED

class Camera
{
public:
	// ���_�̈ʒu���擾����
	const DirectX::SimpleMath::Vector3& GetEyePosition() const
	{
		return m_eyePosition;
	}
	// ���_�̈ʒu��ݒ肷��
	void SetEyePosition(const DirectX::SimpleMath::Vector3& eyePosition)
	{
		m_eyePosition = eyePosition;
	}
	// �����_�̈ʒu���擾����
	DirectX::SimpleMath::Vector3& GetTargetPosition()
	{
		return m_targetPosition;
	}

	// �R���X�g���N�^
	Camera();
	// �f�X�g���N�^
	~Camera();
	// �X�V����
	void Update();

private:
	// ���_�̈ʒu
	DirectX::SimpleMath::Vector3 m_eyePosition;
	// �����_�̈ʒu
	DirectX::SimpleMath::Vector3 m_targetPosition;
};

#endif		// CAMERA_DEFINED
