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
	UNREFERENCED_PARAMETER(elapsedTime);
 
    // �J�����̒ʏ�̍X�V���������s
    using namespace DirectX::SimpleMath;
    // �J�����̌�����ʒu���v�Z���鏈��
    Vector3 playerToEnemy = enemyPos - playerPos;
    playerToEnemy.Normalize();

	Vector3 cameraPos = playerPos - playerToEnemy * Camera::CAMERA_DIRECTION;
    cameraPos.y = Camera::CAMERA_POSITION_Y;


	m_camera->m_position = cameraPos;

	// �J�����̒����_��G�̈ʒu�ɐݒ�
    m_camera->m_target = enemyPos + playerToEnemy;
    // �J�����̒����_�̍�����ݒ肷��
    m_camera->m_target.y = m_camera->m_targetHeight;



    // �J�����̐U������
    m_camera->Shake(0.01667f); // ���̌o�ߎ���

    // �r���[�s����X�V
    m_camera->CalculateViewMatrix();
    m_camera->CalculateCameraAngle();
}


// �X�e�[�g�ύX�iout)
void PlayCameraState::PostUpdate()
{
}

