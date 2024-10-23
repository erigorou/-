// -----------------------------
// �J������ǉ�
// -----------------------------

#include "pch.h"
#include "Game/Camera/Camera.h"
#include "Game/Screen.h"
#include "Libraries/MyLib/Math.h"

#include "Interface/ICameraState.h"
#include "State/TitleCameraState.h"
#include "State/PlayCameraState.h"

// �Œ�l
const float Camera::CAMERA_POSITION_Y = 8.0f;		// �J�����̍���
const float Camera::CAMERA_DIRECTION = 20.0f;		// �J�����̋���

//-------------------------------------------------------------------
/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="target">�����_</param>
//-------------------------------------------------------------------
Camera::Camera(const DirectX::SimpleMath::Vector3& target)
	:m_view{}
	,m_projection{}
	,m_eye{}
	,m_target{ target }
	,m_up{ DirectX::SimpleMath::Vector3::UnitY }
	,m_position{}
	,m_angle{}
	,m_targetHeight{5.0f}
	, m_isShake{ false }
	, m_shakeTime{ SHAKE_TIME }
{
	CreateState();
}

//-------------------------------------------------------------------
/// <summary>
/// �J�����̍X�V����
/// </summary>
/// <param name="playerPos">�v���C���[�̍��W</param>
/// <param name="enemyPos">����������W</param>
/// <param name="rotate">�v���C���[�̉�]��/param>
/// <param name="elapsedTime">�o�ߎ���</param>
//-------------------------------------------------------------------
void Camera::Update(
	const DirectX::SimpleMath::Vector3& playerPos,
	const DirectX::SimpleMath::Vector3& enemyPos,
	const DirectX::SimpleMath::Matrix&	rotate,
	const float							elapsedTime
)
{
	// �X�e�[�g���X�V
	m_currentState->Update(
		playerPos,
		enemyPos,
		rotate,
		elapsedTime
	);

}

//-------------------------------------------------------------------
/// <summary>
/// �J�����̃r���[�s���
/// </summary>
//-------------------------------------------------------------------
void Camera::CalculateViewMatrix()
{
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_position, m_target, m_up);
}

//-------------------------------------------------------------------
/// <summary>
/// �J�����̃A���O�����v�Z����
/// </summary>
//-------------------------------------------------------------------
void Camera::CalculateCameraAngle()
{
	using namespace DirectX::SimpleMath;

	// �J�����̑O�����x�N�g��
	Vector3 forward = m_target - m_position;
	forward.Normalize();

	// ���E���W�n�̑O�����x�N�g��
	Vector3 worldForward = Vector3::Forward;

	// ���ς��v�Z
	float dotProduct = forward.Dot(worldForward);

	// ���ς���p�x���v�Z�i�ʓx�@�j
	m_angle = acosf(dotProduct);

	// �J�����̑O�����x�N�g�����E�����Ɍ����Ă��邩�ǂ����ŕ���������
	Vector3 crossProduct = forward.Cross(worldForward);
	if (crossProduct.y < 0)
	{
		m_angle = -m_angle;
	}
}


// -----------------------------
// �J�����̐U��
// -----------------------------
void Camera::Shake(float elapsedTime)
{
	using namespace DirectX;

	if ( ! m_isShake ) return;

	m_shakeTime -= elapsedTime;

	// �U�����Ԃ�0�ȉ��ɂȂ�����U�����I��
	if (m_shakeTime <= 0.0f)
	{
		m_isShake = false;  
		return;
	}

	float power = m_shakeTime * 1.0f;

	SimpleMath::Vector3 max = SimpleMath::Vector3(power, power, power);
	SimpleMath::Vector3 min = SimpleMath::Vector3(-power, -power, -power);

	// �J�����̈ʒu��h�炷
	m_position	+=	Math::RandomVector3(min, max);
	m_target	+=	Math::RandomVector3(min, max);
}


//-------------------------------------------------------------------
// �X�e�[�g�̕ύX
//-------------------------------------------------------------------
void Camera::ChangeState(ICameraState* state)
{
	// �X�e�[�g�������Ȃ�ύX���Ȃ�
	if (m_currentState == state) return;

	// �X�e�[�g�̕ύX�葱��
	m_currentState->PostUpdate();
	m_currentState = state;
	m_currentState->PreUpdate();
}



//-------------------------------------------------------------------
// �J�����̐U�����J�n
//-------------------------------------------------------------------
void Camera::CreateState()
{
	m_titleState	= std::make_unique<TitleCameraState>(this);
	m_playState		= std::make_unique<PlayCameraState>(this);


	m_currentState = m_titleState.get();
}


//-------------------------------------------------------------------
// �J�����̐U�����J�n
//-------------------------------------------------------------------
void Camera::SetShake()
{
	m_isShake = true;
	m_shakeTime = SHAKE_TIME;
}