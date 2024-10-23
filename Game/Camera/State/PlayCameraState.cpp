#include "pch.h"
#include "PlayCameraState.h"
#include "Interface/ICameraState.h"
#include "../Camera.h"


// �R���X�g���N�^
PlayCameraState::PlayCameraState(Camera* camera)
	: m_camera(camera)
{
}


// �f�X�g���N�^
PlayCameraState::~PlayCameraState()
{
}


// �X�e�[�g�ύX�iin�j
void PlayCameraState::PreUpdate()
{
}


// �X�V����
void PlayCameraState::Update(
	const DirectX::SimpleMath::Vector3& playerPos,
	const DirectX::SimpleMath::Vector3& enemyPos,
	const DirectX::SimpleMath::Matrix& rotate,
	float elapsedTime
)
{
	UNREFERENCED_PARAMETER(rotate);

    // �J�����̒ʏ�̍X�V���������s
    using namespace DirectX::SimpleMath;

    // �J�����̌�����ʒu���v�Z���鏈��
    Vector3 playerToEnemy = enemyPos - playerPos;
    playerToEnemy.Normalize();

    m_camera->m_position = playerPos - playerToEnemy * Camera::CAMERA_DIRECTION;
    m_camera->m_position.y = Camera::CAMERA_POSITION_Y;

    m_camera->m_target = enemyPos + playerToEnemy;

    // �J�����̐U������
    m_camera->Shake(0.016f); // ���̌o�ߎ���

	m_camera->m_target.y = m_camera->m_targetHeight;

    // �r���[�s����X�V
    m_camera->CalculateViewMatrix();
    m_camera->CalculateCameraAngle();
}


// �X�e�[�g�ύX�iout)
void PlayCameraState::PostUpdate()
{
}

