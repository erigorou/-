#include "pch.h"
#include "PlayCameraState.h"
#include "Interface/ICameraState.h"
#include "../Camera.h"
#include "Libraries/MyLib/Math.h"

// ----------------------------
// �R���X�g���N�^
// ----------------------------
PlayCameraState::PlayCameraState(Camera* camera)
	: m_camera(camera)
{
}

// ----------------------------
// �f�X�g���N�^
// ----------------------------
PlayCameraState::~PlayCameraState()
{
}

// ----------------------------
// �X�e�[�g�ύX�iin�j
// ----------------------------
void PlayCameraState::PreUpdate()
{
}

// ----------------------------
// �X�V����
// ----------------------------
void PlayCameraState::Update(
	const DirectX::SimpleMath::Vector3& playerPos,
	const DirectX::SimpleMath::Vector3& enemyPos,
	float elapsedTime
)
{
	// �v���C���[����G�ւ̕����x�N�g�����v�Z
	Vector3 playerToEnemy = enemyPos - playerPos;
	playerToEnemy.Normalize();
	// �v���C���ƓG�̋������擾
	float distance = Vector3::Distance(playerPos, enemyPos);
	// �����ɂ���ăJ�����̍�����ύX
	float normalizedDistance = std::min(std::max((distance / STAGE_LENGTH), 0.8f), 1.2f);
	// �J�����̖ڕW�ʒu���v�Z
	Vector3 targetCameraPos = playerPos - playerToEnemy * Camera::CAMERA_DIRECTION;
	// �����������ˑ��ɕύX
	targetCameraPos.y = Camera::CAMERA_POSITION_Y * normalizedDistance;
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

// ----------------------------
// �X�e�[�g�ύX�iout)
// ----------------------------
void PlayCameraState::PostUpdate()
{
}