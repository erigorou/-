#pragma once

#ifndef PLAY_CAMERA_STATE
#define PLAY_CAMERA_STATE

#include "pch.h"
#include "Interface/ICameraState.h"
#include "../Camera.h"

class Camera;

class PlayCameraState : public ICameraState
{
// ----------------------------
// �萔
// ----------------------------
public:
	// �X�e�[�W�̒���
	static constexpr float STAGE_LENGTH = 100;

// ----------------------------
// �����o�֐�
// ----------------------------
public:
	// �R���X�g���N�^
	PlayCameraState(Camera* camera);
	// �f�X�g���N�^
	~PlayCameraState()override;
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
	Camera* m_camera;
};


#endif // !PLAY_CAMERA_STATE
