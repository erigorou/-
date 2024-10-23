#pragma once

#ifndef PLAY_CAMERA_STATE
#define PLAY_CAMERA_STATE

#include "pch.h"
#include "Interface/ICameraState.h"
#include "../Camera.h"

class Camera;

class PlayCameraState : public ICameraState
{
public:
	PlayCameraState(Camera* camera);
	~PlayCameraState()override;

	void PreUpdate()override;

	void Update(
		const DirectX::SimpleMath::Vector3& playerPos,
		const DirectX::SimpleMath::Vector3& enemyPos,
		const DirectX::SimpleMath::Matrix& rotate,
		float elapsedTime
	)override;

	void PostUpdate()override;

private:
	Camera* m_camera;
};


#endif // !PLAY_CAMERA_STATE
