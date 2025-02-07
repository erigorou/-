// ---------------------------------------------
// 名前:	TitleCameraState.h
// 内容:	タイトル時のカメラステートクラス
// 作成:	池田桜輔
// ---------------------------------------------
#pragma once
#ifndef TITLE_CAMERA_STATE
#define TITLE_CAMERA_STATE

// インクルード
#include "pch.h"
#include "Interface/ICameraState.h"

// 前方宣言
class Camera;

class TitleCameraState : public ICameraState
{
// ----------------------------
// 定数
// ----------------------------
private:
	// カメラのY座標
	static constexpr DirectX::SimpleMath::Vector3 CAMERA_Y_PLUS = { 0.0f, 40.0f, 0.0f };
	// タイトルでのカメラの距離
	static constexpr DirectX::SimpleMath::Vector3 TITLE_DIRECTION = { 125.0f, 10.0f, 125.0f };
	// 回転速度
	static constexpr float ROTATE_SPEED = -0.1f;

// ----------------------------
// メンバ関数
// ----------------------------
public:
	// コンストラクタ
	TitleCameraState(Camera* camera);
	// デストラクタ
	~TitleCameraState()override;
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
	// 角度
	float m_angle;
};

#endif