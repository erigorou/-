// -----------------------------
// �J������ǉ�
// -----------------------------

#include "pch.h"
#include "Game/Camera/Camera.h"
#include "Game/Screen.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Messenger/EventMessenger.h"

#include "Interface/ICameraState.h"
#include "State/TitleCameraState.h"
#include "State/PlayCameraState.h"

//-------------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------------
Camera::Camera(const DirectX::SimpleMath::Vector3& target)
	:m_view{}
	,m_projection{}
	,m_target{ target }
	,m_up{ DirectX::SimpleMath::Vector3::UnitY }
	,m_position{}
	,m_angle{}
	,m_targetHeight{TARGET_HEIGHT}
	, m_isShake{ false }
	, m_shakeTime{ SHAKE_TIME }
	, m_shakePos{}
{
	// �X�e�[�g���쐬
	CreateState();

	// �J�����̃V�F�C�N���C�x���g�ɓo�^
	EventMessenger::Attach(EventList::ShakeCamera, std::bind(&Camera::SetShake, this, std::placeholders::_1));
}

//-------------------------------------------------------------------
// �J�����̍X�V����
//-------------------------------------------------------------------
void Camera::Update(
	const DirectX::SimpleMath::Vector3& playerPos,
	const DirectX::SimpleMath::Vector3& enemyPos,
	const float	elapsedTime
)
{
	// �X�e�[�g���X�V
	m_currentState->Update(
		playerPos,
		enemyPos,
		elapsedTime
	);
}

//-------------------------------------------------------------------
// �J�����̃r���[�s����v�Z����
//-------------------------------------------------------------------
void Camera::CalculateViewMatrix()
{
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_position, m_target, m_up);
}


//-------------------------------------------------------------------
// �J�����̃A���O�����v�Z����
//-------------------------------------------------------------------
void Camera::CalculateCameraAngle()
{
	// �J�����̑O�����x�N�g��
	DirectX::SimpleMath::Vector3 forward = m_target - m_position;
	forward.Normalize();

	// ���E���W�n�̑O�����x�N�g��
	DirectX::SimpleMath::Vector3 worldForward = DirectX::SimpleMath::Vector3::Forward;

	// ���ς��v�Z
	float dotProduct = forward.Dot(worldForward);

	// ���ς���p�x���v�Z�i�ʓx�@�j
	float targetAngle = acosf(dotProduct);

	// �J�����̑O�����x�N�g�����E�����Ɍ����Ă��邩�ǂ����ŕ���������
	DirectX::SimpleMath::Vector3 crossProduct = forward.Cross(worldForward);
	if (crossProduct.y < 0)
	{
		targetAngle = -targetAngle;
	}

	// ���`��ԂŌ��݂̃A���O�����X�V
	m_currentAngle = Math::LerpFloat(m_currentAngle, targetAngle, CAMERA_TARGET_RATE);

	// �X�V���ꂽ�A���O���𔽉f
	m_angle = m_currentAngle;
}


// -----------------------------
// �J�����̐U��
// -----------------------------
void Camera::Shake(float elapsedTime)
{
	if ( ! m_isShake ) return;

	m_shakeTime -= elapsedTime;

	// �U�����Ԃ�0�ȉ��ɂȂ�����U�����I��
	if (m_shakeTime <= 0.0f)
	{
		m_isShake = false;  
		return;
	}

	// shakeTime�ɉ����ĐU���̋���������
	float power = (m_shakeTime / SHAKE_TIME) * m_shakePower;

	DirectX::SimpleMath::Vector3 max = DirectX::SimpleMath::Vector3(power	, power	, power	);
	DirectX::SimpleMath::Vector3 min = DirectX::SimpleMath::Vector3(-power, -power, -power);

	// �J�����̈ʒu��h�炷
	m_shakePos	=	Math::RandomVector3(min, max);
	m_target   +=	Math::RandomVector3(min, max);
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
void Camera::SetShake(void* power)
{
	m_isShake = true;
	m_shakeTime = SHAKE_TIME;
	m_shakePower = *static_cast<float*>(power);
}