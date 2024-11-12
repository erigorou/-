#include "pch.h"

#include "TitleCameraState.h"
#include "../Camera.h"

// �R���X�g���N�^
TitleCameraState::TitleCameraState(Camera* camera)
	: m_camera(camera)
	, m_angle(0.0f)
{
}

// �f�X�g���N�^
TitleCameraState::~TitleCameraState()
{
}


// �X�e�[�g�ύX�iin�j
void TitleCameraState::PreUpdate()
{
}


// �X�V����
void TitleCameraState::Update(
    const DirectX::SimpleMath::Vector3& playerPos,
    const DirectX::SimpleMath::Vector3& enemyPos,
    const DirectX::SimpleMath::Matrix& rotate,
    float elapsedTime
)
{
    UNREFERENCED_PARAMETER(playerPos);
    UNREFERENCED_PARAMETER(enemyPos);
    UNREFERENCED_PARAMETER(rotate);

    // ��]
    m_angle += ROTATE_SPEED * elapsedTime; // ���Ԃɉ����Ċp�x���X�V
    // �J�����̈ʒu���~�^���Ɋ�Â��Čv�Z
    m_camera->m_position.x = TITLE_DIRECTION.x * cosf(m_angle) + m_camera->m_shakePos.x; // x���W
    m_camera->m_position.z = TITLE_DIRECTION.z * sinf(m_angle) + m_camera->m_shakePos.z; // z���W
    m_camera->m_position.y = TITLE_DIRECTION.y + m_camera->m_shakePos.y;                 // �������Œ�
    // �J�����̒����_�𒆐S�ɐݒ�
    m_camera->m_target = DirectX::SimpleMath::Vector3::Zero + CAMERA_Y_PLUS + m_camera->m_shakePos;

    // �r���[�s����X�V
    m_camera->CalculateViewMatrix();
    m_camera->CalculateCameraAngle();
}


// �X�e�[�g�ύX�iout)
void TitleCameraState::PostUpdate()
{
}
