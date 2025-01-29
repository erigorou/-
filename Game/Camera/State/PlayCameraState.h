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
// 定数
// ----------------------------
public:
	// ステージの長さ
	static constexpr float STAGE_LENGTH = 100;

// ----------------------------
// メンバ関数
// ----------------------------
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
		float elapsedTime
	)override;
	// ステート変更（out）
	void PostUpdate()override;

// ----------------------------
// メンバ変数
// ----------------------------
private:
	Camera* m_camera;
};


#endif // !PLAY_CAMERA_STATE
