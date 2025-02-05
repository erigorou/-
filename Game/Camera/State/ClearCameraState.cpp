#include "pch.h"
#include "ClearCameraState.h"

#include "../Camera.h"
#include "Libraries/MyLib/Math.h"

// ---------------------------------------------
/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="camera">�J�����I�u�W�F�N�g</param>
// ---------------------------------------------
ClearCameraState::ClearCameraState(Camera* camera)
	: 
	m_camera(camera),
	m_totalTime{}
{
}


// ---------------------------------------------
/// <summary>
/// �f�X�g���N�^
/// </summary>
// ---------------------------------------------
ClearCameraState::~ClearCameraState()
{
}

// ---------------------------------------------
/// <summary>
/// �X�e�[�g�̑O�X�V����
/// </summary>
// ---------------------------------------------
void ClearCameraState::PreUpdate()
{
}

// ---------------------------------------------
/// <summary>
/// �X�e�[�g�̍X�V����
/// </summary>
/// <param name="playerPos">�v���C���[�̍��W</param>
/// <param name="enemyPos">�G�̍��W</param>
/// <param name="elapsedTime">�o�ߎ���</param>
// ---------------------------------------------
void ClearCameraState::Update(
	const DirectX::SimpleMath::Vector3& playerPos,
	const DirectX::SimpleMath::Vector3& enemyPos,
	float elapsedTime
)
{
	// ���Ԃ����Z
	m_totalTime += elapsedTime;

	// �J�����̉�]���x�i�������j
	const float rotationSpeed = 0.2f;

	// �ő厞�ԁi���o���ԁj
	const float maxTime = 3.0f;

	// �ŏ��̃J���������i�J�n���̋����j
	const float startDistance = 10.0f;
	// �ő�J���������i�Y�[���A�E�g���̋����j
	const float maxDistance = 20.0f;

	// ���X�ɉ������鋗�����v�Z�iLerp�j
	float distanceFactor = std::min(m_totalTime / maxTime, 1.0f);
	float currentDistance = startDistance + (maxDistance - startDistance) * distanceFactor;

	// �J�����̊p�x���v�Z�i�~�^���j
	float angle = rotationSpeed * m_totalTime;
	float offsetX = cosf(angle) * currentDistance;
	float offsetZ = sinf(angle) * currentDistance;

	// �J�����̍����i�������㏸�j
	float heightOffset = 10.0f + (10.0f * distanceFactor);

	// �G�𒆐S�ɃJ������z�u
	Vector3 targetCameraPos = Vector3(offsetX, heightOffset, offsetZ);

	// �J�����ʒu���Ԃ��ĒǏ]
	m_camera->m_position = Math::LerpVector(m_camera->m_position, targetCameraPos, 0.02f);

	// �����_��G�̍��W�ɌŒ�
	m_camera->m_target = Math::LerpVector(m_camera->m_target, enemyPos, 0.05f);

	// �r���[�s��̍X�V
	m_camera->CalculateViewMatrix();
}



// ---------------------------------------------
/// <summary>
/// �X�e�[�g�̌�X�V����
/// </summary>
// ---------------------------------------------
void ClearCameraState::PostUpdate()
{
}