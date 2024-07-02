// --------------
// �J������ǉ�
//
// --------------

#include "pch.h"
#include "Game/Camera/Camera.h"
#include "Game/Screen.h"


//-------------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------------
Camera::Camera(const DirectX::SimpleMath::Vector3& target)
	:
	m_view{},
	m_projection{},
	m_eye{},
	m_target{ target },
	m_up{ DirectX::SimpleMath::Vector3::UnitY },
	m_position{},
	m_angle{}
{
}

//-------------------------------------------------------------------
/// <summary>
/// �J�����̍X�V����
/// </summary>
/// <param name="playerPos">�v���C���[�̍��W</param>
/// <param name="enemyPos">����������W</param>
/// <param name="rotate">�v���C���[�̉�]��/param>
//-------------------------------------------------------------------
void Camera::Update(
	const DirectX::SimpleMath::Vector3& playerPos,
	const DirectX::SimpleMath::Vector3& enemyPos,
	const DirectX::SimpleMath::Matrix& rotate
)
{	using namespace DirectX;

	float distance = 50.0f;

	// �v���C���[����^�[�Q�b�g�ւ̃x�N�g�����v�Z
	SimpleMath::Vector3 playerToEnemy = enemyPos - playerPos;
	// �x�N�g���𐳋K�����ĒP�ʃx�N�g�����擾
	SimpleMath::Vector3 unitVecPlayerToTarget = playerToEnemy;
	unitVecPlayerToTarget.Normalize();
	// �J�����ʒu���v�Z
	m_position = playerPos - unitVecPlayerToTarget * distance;
	m_position.y = 20.0f; // Y���W���Œ�
	// �����_���v�Z
	m_target = enemyPos + unitVecPlayerToTarget;
	// �����_�̍�����ݒ�
	m_target.y = 0.5f;
	// �r���[�s����X�V����
	CalculateViewMatrix();
	// �J�����̃A���O�����v�Z����
	CalculateCameraAngle();
}

//-------------------------------------------------------------------
// �r���[�s����v�Z����
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



