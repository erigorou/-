#include "pch.h"
#include "PlayCameraState.h"
#include "Interface/ICameraState.h"
#include "../Camera.h"
#include "Libraries/MyLib/Math.h"

// ----------------------------
// コンストラクタ
// ----------------------------
PlayCameraState::PlayCameraState(Camera* camera)
	: m_camera(camera)
{
}

// ----------------------------
// デストラクタ
// ----------------------------
PlayCameraState::~PlayCameraState()
{
}

// ----------------------------
// ステート変更（in）
// ----------------------------
void PlayCameraState::PreUpdate()
{
}

// ----------------------------
// 更新処理
// ----------------------------
void PlayCameraState::Update(
	const DirectX::SimpleMath::Vector3& playerPos,
	const DirectX::SimpleMath::Vector3& enemyPos,
	float elapsedTime
)
{
	// プレイヤーから敵への方向ベクトルを計算
	Vector3 playerToEnemy = enemyPos - playerPos;
	playerToEnemy.Normalize();
	// プレイヤと敵の距離を取得
	float distance = Vector3::Distance(playerPos, enemyPos);
	// 距離によってカメラの高さを変更
	float normalizedDistance = std::min(std::max((distance / STAGE_LENGTH), 0.8f), 1.2f);
	// カメラの目標位置を計算
	Vector3 targetCameraPos = playerPos - playerToEnemy * Camera::CAMERA_DIRECTION;
	// 高さを距離依存に変更
	targetCameraPos.y = Camera::CAMERA_POSITION_Y * normalizedDistance;
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

// ----------------------------
// ステート変更（out)
// ----------------------------
void PlayCameraState::PostUpdate()
{
}