#pragma once
#ifndef CLEAR_CAMERA_STATE
#define CLEAR_CAMERA_STATE

#include "pch.h"
#include "Interface/ICameraState.h"

class Camera;

class ClearCameraState : public ICameraState
{
	// ----------------------------
	// �萔
	// ----------------------------
public:




	// ----------------------------
	// �����o�֐�
	// ----------------------------
public:
	// �R���X�g���N�^
	ClearCameraState(Camera* camera);
	// �f�X�g���N�^
	~ClearCameraState()override;
	// �X�e�[�g�ύX�iin�j
	void PreUpdate()override;
	// �X�V����
	void Update(
		const DirectX::SimpleMath::Vector3& playerPos,
		const DirectX::SimpleMath::Vector3& enemyPos,
		float elapsedTime
	)override;
	// �X�e�[�g�ύX�iout�j
	void PostUpdate()override;

	// ----------------------------
	// �����o�ϐ�
	// ----------------------------
private:
	// �J����
	Camera* m_camera;
	// �o�ߎ���
	float m_totalTime;

};

#endif // !CLEAR_CAMERA_STATE