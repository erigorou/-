// ---------------------------------------------
// 名前		:PlayCameraState.h
// 内容		:プレイ時のカメラステートクラス
// 作成		:池田桜輔
// ---------------------------------------------
#pragma once
#ifndef PLAY_CAMERA_STATE
#define PLAY_CAMERA_STATE

// インクルード
#include "pch.h"
#include "Interface/ICameraState.h"

// 前方宣言
class Camera;

/// <summary>
/// プレイシーンのカメラステートクラス
/// </summary>
class PlayCameraState : public ICameraState
{
	// ----------------------------
	// 定数
	// ----------------------------
public:
	// ステージの長さ
	static constexpr float STAGE_LENGTH = 100;
	// カメラの補間量
	static constexpr float LERP_RATE = 0.075f;

	// 物体同士の距離によるカメラの高さの変化
	static constexpr float CAMERA_ZOOM_MIN_FACTOR = 0.8f;
	static constexpr float CAMERA_ZOOM_MAX_FACTOR = 1.2f;


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
	// カメラ
	Camera* m_camera;
};

	#endif // !PLAY_CAMERA_STATE
