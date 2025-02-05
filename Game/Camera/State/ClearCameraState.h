#pragma once
#ifndef CLEAR_CAMERA_STATE
#define CLEAR_CAMERA_STATE

#include "pch.h"
#include "Interface/ICameraState.h"

class Camera;

class ClearCameraState : public ICameraState
{
	// ----------------------------
	// 定数
	// ----------------------------
public:




	// ----------------------------
	// メンバ関数
	// ----------------------------
public:
	// コンストラクタ
	ClearCameraState(Camera* camera);
	// デストラクタ
	~ClearCameraState()override;
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
	// カメラ
	Camera* m_camera;
	// 経過時間
	float m_totalTime;

};

#endif // !CLEAR_CAMERA_STATE