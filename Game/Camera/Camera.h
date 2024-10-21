#pragma once

class ICameraState;

class Camera
{
	class CameraIdling;


public:
	// �Œ�l
	static const float CAMERA_POSITION_Y;			// �J������Y���W
	static const float CAMERA_DIRECTION;			// �J�����̋���
	static constexpr float SHAKE_TIME = 0.5f;		// �J������h�炷����
	static constexpr float MINIMAL = 0.01f;	// �ŏ��l

	const DirectX::SimpleMath::Matrix&		GetViewMatrix		() const { return m_view;		}	// �r���[�s��
	const DirectX::SimpleMath::Matrix&	GetProjectionMatrix		() const { return m_projection; }	// �v���W�F�N�V�����s��
	const DirectX::SimpleMath::Vector3& GetEyePosition			() const { return m_position;	}	// �J�������W
	const DirectX::SimpleMath::Vector3& GetTargetPosition		() const { return m_target;		}	// �����_
	const DirectX::SimpleMath::Vector3& GetUpVector				() const { return m_up;			}	// �J�����̓��̕���
	const float&						GetCameraAngle			() const { return m_angle;		}	// �A���O��



	Camera(const DirectX::SimpleMath::Vector3& target = DirectX::SimpleMath::Vector3::Zero);	// �R���X�g���N�^
	~Camera() = default;																		// �f�X�g���N�^	


	// �X�V����
	void Update(
		const DirectX::SimpleMath::Vector3& playerPos,
		const DirectX::SimpleMath::Vector3& enemyPos,
		const DirectX::SimpleMath::Matrix& rotate,
		const float elapsedTime
	);

	void SetShake();	// �J������h�炷

private:

	void CalculateViewMatrix();			// �r���[�s����v�Z����
	void CalculateCameraAngle();		// �J�����̃A���O�����v�Z����

	void Shake(float elapsedTime);						// �J������h�炷

	DirectX::SimpleMath::Vector3	m_position;		// �J�����ʒu
	DirectX::SimpleMath::Matrix		m_view;			// �r���[�s��
	DirectX::SimpleMath::Matrix		m_projection;	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Vector3	m_eye;			// �J�������W
	DirectX::SimpleMath::Vector3	m_target;		// �����_
	DirectX::SimpleMath::Vector3	m_up;			// �J�����̓��̕���
	

	float m_angle;					// �A���O��
	float m_targetHeight;			// �����_�̍���

	bool m_isShake;					// �J������h�炷���ǂ���
	float m_shakeTime;				// �h��鎞��

	// �X�e�[�g�p ///////////
	ICameraState* m_currentState;	// ���݂̃X�e�[�g

	

};