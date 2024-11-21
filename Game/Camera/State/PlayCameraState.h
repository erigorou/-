#pragma once

#ifndef PLAY_CAMERA_STATE
#define PLAY_CAMERA_STATE

#include "pch.h"
#include "Interface/ICameraState.h"
#include "../Camera.h"

class Camera;

class PlayCameraState : public ICameraState
{
// 固定値
public:
	static constexpr float STAGE_LENGTH = 100;

// 公開関数
public:
	// コンストラクタ
	PlayCameraState(Camera* camera);
	// デストラクタ
	~PlayCameraState()override;
	// ステート変更（in）
	void PreUpdate()override;
	// 更新処理
	void Update(
		const DirectX::SimpleMath::Vector3& playerPos,
		const DirectX::SimpleMath::Vector3& enemyPos,
		const DirectX::SimpleMath::Matrix& rotate,
		float elapsedTime
	)override;
	// ステート変更（out）
	void PostUpdate()override;

private:
	Camera* m_camera;

};


#endif // !PLAY_CAMERA_STATE
