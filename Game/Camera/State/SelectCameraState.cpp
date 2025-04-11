// ---------------------------------------------
// 
// ���O		:SelectCameraState.cpp
// ���e		:�X�e�[�W�I�����̃J�����X�e�[�g�N���X
// �쐬		:�r�c����
// 
// ---------------------------------------------
#pragma once
// �C���N���[�h
#include "pch.h"
#include "SelectCameraState.h"
#include "Game/Camera/Camera.h"
#include "Libraries/MyLib/Math.h"

// ---------------------------------------------
/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="camera">�J�����I�u�W�F�N�g</param>
// ----------------------------------------------
SelectCameraState::SelectCameraState(Camera* camera)
	: m_camera(camera)
	, m_cameraY()
	, m_elapsedTime()
{
}

// ---------------------------------------------
/// <summary>
/// �f�X�g���N�^
/// </summary>
// ----------------------------------------------
SelectCameraState::~SelectCameraState()
{
}

// ---------------------------------------------
/// <summary>
/// �X�e�[�g�̑O�X�V����
/// </summary>
// ----------------------------------------------
void SelectCameraState::PreUpdate()
{
	// �o�ߎ��Ԃ����Z�b�g
	m_elapsedTime = 0.0f;
}


// ---------------------------------------------
/// <summary>
/// �X�e�[�g�̍X�V����
/// </summary>
/// <param name="playerPos">�v���C���[�̍��W</param>
/// /// <param name="enemyPos">�G�̍��W</param>
/// /// <param name="elapsedTime">�o�ߎ���</param>
// ----------------------------------------------
void SelectCameraState::Update(
	const DirectX::SimpleMath::Vector3& playerPos,
	const DirectX::SimpleMath::Vector3& enemyPos,
	float elapsedTime
)
{
	// �o�ߎ��Ԃ����Z
	m_elapsedTime += elapsedTime;
	// �J������Y���W��sin�g�ŏ㉺������
	m_cameraY = SINE_HEIGHT * sinf(m_elapsedTime / SINE_PERIOD);

	// �v���C���[����G�ւ̕����x�N�g�����v�Z
	Vector3 playerToEnemy = enemyPos - playerPos;
	playerToEnemy.Normalize();
	// �J�����̖ڕW�ʒu���v�Z
	Vector3 targetCameraPos = playerPos - playerToEnemy * Camera::CAMERA_DIRECTION;
	// �����������ˑ��ɕύX
	targetCameraPos.y = Camera::CAMERA_POSITION_Y + m_cameraY;
	// �J�����ʒu���Ԃ��ĒǏ]
	float followSpeed = LERP_RATE;
	// �J�����̈ʒu���Ԃ��ĒǏ]
	m_camera->m_position = Math::LerpVector(m_camera->m_position, targetCameraPos, followSpeed);
	// �����_�̖ڕW�ʒu���v�Z
	Vector3 targetLookAt = enemyPos;
	targetLookAt.y = m_camera->m_targetHeight;
	// �����_���Ԃ��ĒǏ]
	m_camera->m_target = Math::LerpVector(m_camera->m_target, targetLookAt, followSpeed);
	// �J�����̐U�������i�K�v�Ȃ�U�����Ԃ�n���j
	m_camera->Shake(elapsedTime);
	// �r���[�s��ƃA���O���̍X�V
	m_camera->CalculateViewMatrix();
	m_camera->CalculateCameraAngle();
}

// ---------------------------------------------
/// <summary>
/// �X�e�[�g�̌�X�V����
/// </summary>
// ---------------------------------------------
void SelectCameraState::PostUpdate()
{
}