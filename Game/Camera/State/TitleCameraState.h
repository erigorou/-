#pragma once

#ifndef TITLE_CAMERA_STATE
#define TITLE_CAMERA_STATE

#include "pch.h"

#include "Interface/ICameraState.h"

class Camera;


class TitleCameraState : public ICameraState
{
private:
	static constexpr DirectX::SimpleMath::Vector3	CAMERA_Y_PLUS		= { 0.0f,	40.0f,	0.0f	};
	static constexpr DirectX::SimpleMath::Vector3	TITLE_DIRECTION		= { 125.0f, 10.0f,	125.0f	};
	static constexpr float							ROTATE_SPEED		=	-0.1f;


public:
	TitleCameraState(Camera* camera);
	~TitleCameraState()override;

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
	float m_angle;
};

#endif // !TITLE_CAMERA_STATE