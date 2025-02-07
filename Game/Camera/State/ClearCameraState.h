// ---------------------------------------------
// 名前:	ClearCameraState.h
// 内容:	クリア時のカメラステートクラス
// 作成:	池田桜輔
// ---------------------------------------------
#pragma once
#ifndef CLEAR_CAMERA_STATE
#define CLEAR_CAMERA_STATE

// インクルード
#include "pch.h"
#include "Interface/ICameraState.h"

// 前方宣言
class Camera;

/// <summary>
/// クリア時のカメラステートクラス
/// </summary>
class ClearCameraState : public ICameraState
{
	// ----------------------------
	// 定数
	// ----------------------------
public:
	// カメラの回転速度
	static constexpr float ROTATION_SPEED = 0.2f;
	// 最大時間（演出時間）
	static constexpr float MAX_TIME = 3.0f;
	// 最初のカメラ距離（開始時の距離）
	static constexpr float START_DISTANCE = 10.0f;
	// 最大カメラ距離（ズームアウト時の距離）
	static constexpr float MAX_DISTANCE = 20.0f;
	//　カメラの高さ
	static constexpr float CAMERA_HEIGHT = 10.0f;


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