#include "Framework/pch.h"
#include "Framework/Camera.h"

// �R���X�g���N�^
Camera::Camera()
	:
	m_eyePosition(DirectX::SimpleMath::Vector3(1.5f, 4.5f, 1.5f)),		// ���_�̈ʒu
	m_targetPosition(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f))		// �����_�̈ʒu
{
}

// �f�X�g���N�^
Camera::~Camera()
{
}

// �r���[�s����X�V����
void Camera::Update()
{
	// �J������UP�x�N�g�����v�Z����
	DirectX::SimpleMath::Vector3 toEye = GetEyePosition() - GetTargetPosition();
	// ���K������
	toEye.Normalize();
	// UP�x�N�g�����v�Z����
	DirectX::SimpleMath::Vector3 up = toEye.Cross(DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f));
	// �r���[�s��𐶐�����
	DirectX::SimpleMath::Matrix view = DirectX::SimpleMath::Matrix::CreateLookAt(GetEyePosition(), GetTargetPosition(), up	);
	// �r���[�s���ݒ肷��
	Graphics::GetInstance()->SetViewMatrix(view);
}
