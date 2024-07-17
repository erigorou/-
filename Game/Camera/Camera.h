#pragma once

class Camera
{
private:
	// �Œ�l
	static const float CAMERA_POSITION_Y;		// �J������Y���W
	static const float CAMERA_DIRECTION;		// �J�����̋���

	// �J�����̍��W
	DirectX::SimpleMath::Vector3 m_position;
	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix m_projection;
	// �J�������W
	DirectX::SimpleMath::Vector3 m_eye;
	// �����_
	DirectX::SimpleMath::Vector3 m_target;
	// �J�����̓��̕���
	DirectX::SimpleMath::Vector3 m_up;
	// �A���O��
	float m_angle;
	// �����_�̍���
	float m_targetHeight;


public:
	// �R���X�g���N�^
	Camera(const DirectX::SimpleMath::Vector3& target = DirectX::SimpleMath::Vector3::Zero);
	// �f�X�g���N�^
	~Camera() = default;

	// �X�V����
	void Update(
		const DirectX::SimpleMath::Vector3& playerPos,
		const DirectX::SimpleMath::Vector3& enemyPos,
		const DirectX::SimpleMath::Matrix& rotate
	);


	// getter
	// �r���[�s��
	const DirectX::SimpleMath::Matrix& GetViewMatrix() const { return m_view; }
	// �v���W�F�N�V����
	const DirectX::SimpleMath::Matrix& GetProjectionMatrix() const { return m_projection; }
	// �J�������W
	const DirectX::SimpleMath::Vector3& GetEyePosition() const { return m_position; }
	// �����_
	const DirectX::SimpleMath::Vector3& GetTargetPosition() const { return m_target; }
	// �����Ă������
	const DirectX::SimpleMath::Vector3& GetUpVector() const { return m_up; }
	// �J�����̃A���O��
	const float& GetCameraAngle() const { return m_angle; }


private:
	// �r���[�s����v�Z����
	void CalculateViewMatrix();

	// �J�����̃A���O�����v�Z����
	void CalculateCameraAngle();
};