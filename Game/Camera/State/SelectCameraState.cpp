// ---------------------------------------------
// 
// 名前		:SelectCameraState.cpp
// 内容		:ステージ選択時のカメラステートクラス
// 作成		:池田桜輔
// 
// ---------------------------------------------
#pragma once
// インクルード
#include "pch.h"
#include "SelectCameraState.h"
#include "Game/Camera/Camera.h"
#include "Libraries/MyLib/Math.h"

// ---------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="camera">カメラオブジェクト</param>
// ----------------------------------------------
SelectCameraState::SelectCameraState(Camera* camera)
	: m_camera(camera)
	, m_cameraY()
	, m_elapsedTime()
{
}

// ---------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ----------------------------------------------
SelectCameraState::~SelectCameraState()
{
}

// ---------------------------------------------
/// <summary>
/// ステートの前更新処理
/// </summary>
// ----------------------------------------------
void SelectCameraState::PreUpdate()
{
	// 経過時間をリセット
	m_elapsedTime = 0.0f;
}


// ---------------------------------------------
/// <summary>
/// ステートの更新処理
/// </summary>
/// <param name="playerPos">プレイヤーの座標</param>
/// /// <param name="enemyPos">敵の座標</param>
/// /// <param name="elapsedTime">経過時間</param>
// ----------------------------------------------
void SelectCameraState::Update(
	const DirectX::SimpleMath::Vector3& playerPos,
	const DirectX::SimpleMath::Vector3& enemyPos,
	float elapsedTime
)
{
	// 経過時間を加算
	m_elapsedTime += elapsedTime;
	// カメラのY座標をsin波で上下させる
	m_cameraY = SINE_HEIGHT * sinf(m_elapsedTime / SINE_PERIOD);

	// プレイヤーから敵への方向ベクトルを計算
	Vector3 playerToEnemy = enemyPos - playerPos;
	playerToEnemy.Normalize();
	// カメラの目標位置を計算
	Vector3 targetCameraPos = playerPos - playerToEnemy * Camera::CAMERA_DIRECTION;
	// 高さを距離依存に変更
	targetCameraPos.y = Camera::CAMERA_POSITION_Y + m_cameraY;
	// カメラ位置を補間して追従
	float followSpeed = LERP_RATE;
	// カメラの位置を補間して追従
	m_camera->m_position = Math::LerpVector(m_camera->m_position, targetCameraPos, followSpeed);
	// 注視点の目標位置を計算
	Vector3 targetLookAt = enemyPos;
	targetLookAt.y = m_camera->m_targetHeight;
	// 注視点を補間して追従
	m_camera->m_target = Math::LerpVector(m_camera->m_target, targetLookAt, followSpeed);
	// カメラの振動処理（必要なら振動時間を渡す）
	m_camera->Shake(elapsedTime);
	// ビュー行列とアングルの更新
	m_camera->CalculateViewMatrix();
	m_camera->CalculateCameraAngle();
}

// ---------------------------------------------
/// <summary>
/// ステートの後更新処理
/// </summary>
// ---------------------------------------------
void SelectCameraState::PostUpdate()
{
}