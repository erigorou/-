#pragma once

#include "State/TitleCameraState.h"
#include "State/PlayCameraState.h"

class ICameraState;
class TitleCameraState;
class PlayCameraState;

class Camera
{

public:
	// �Œ�l
	static constexpr float CAMERA_POSITION_Y	= 10.0f;	// �J������Y���W
	static constexpr float CAMERA_DIRECTION		= 30.0f;	// �J�����̋���
	static constexpr float TARGET_HEIGHT		= 8.0f;		// �����_�̍���

	static constexpr float SHAKE_TIME	= 0.5f;		// �J������h�炷����
	static constexpr float SHAKE_POWER	= 0.5f;		// �J�����̂��鋭��
	static constexpr float MINIMAL		= 0.01f;	// �ɏ��l

	static constexpr float CAMERA_EYE_RATE		= 0.5f;	// �J�����̒Ǐ]�W��
	static constexpr float CAMERA_TARGET_RATE	= 0.1f;	// �^�[�Q�b�g�̒Ǐ]�W��

	// �X�e�[�g
	TitleCameraState*	GetTitleState()	{ return m_titleState.get();	}
	PlayCameraState*	GetPlayState()	{ return m_playState.get();		}


	const DirectX::SimpleMath::Matrix&	GetViewMatrix			() const { return m_view;		}	// �r���[�s��
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

	void SetShake(void* power);			// �J������h�炷 // �����ɂ̓t���[�g�^�̃|�C���^��n��

	void CalculateViewMatrix();			// �r���[�s����v�Z����
	void CalculateCameraAngle();		// �J�����̃A���O�����v�Z����

	void Shake(float elapsedTime);						// �J������h�炷

	void ChangeState(ICameraState* state);	// �X�e�[�g�̕ύX

	DirectX::SimpleMath::Vector3	m_position;		// �J�����ʒu
	DirectX::SimpleMath::Matrix		m_view;			// �r���[�s��
	DirectX::SimpleMath::Matrix		m_projection;	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Vector3	m_eye;			// �J�������W
	DirectX::SimpleMath::Vector3	m_target;		// �����_
	DirectX::SimpleMath::Vector3	m_up;			// �J�����̓��̕���
	
	float m_currentAngle;			// ���݂̃A���O��
	float m_angle;					// �A���O��
	float m_targetHeight;			// �����_�̍���

	bool m_isShake;					// �J������h�炷���ǂ���
	float m_shakeTime;				// �h��鎞��

	DirectX::SimpleMath::Vector3 m_shakePos;	// �h�����W
	float m_shakePower;							// �h��鋭��

private:

	void CreateState();	// �X�e�[�g�̐���

	// �X�e�[�g�p ///////////
	ICameraState* m_currentState;	// ���݂̃X�e�[�g

	std::unique_ptr<TitleCameraState>	m_titleState;
	std::unique_ptr<PlayCameraState>	m_playState;



};