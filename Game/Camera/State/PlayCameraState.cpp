#include "pch.h"
#include "PlayCameraState.h"

#include "../Camera.h"
#include "Libraries/MyLib/Math.h"

// ---------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="camera">カメラオブジェクト</param>
// ---------------------------------------------
PlayCameraState::PlayCameraState(Camera* camera)
	: m_camera(camera)
{
}

// ---------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------------
PlayCameraState::~PlayCameraState()
{
}

// ---------------------------------------------
/// <summary>
/// ステートの前更新処理
/// </summary>
// ---------------------------------------------
void PlayCameraState::PreUpdate()
{
}

// ---------------------------------------------
/// <summary>
/// ステートの更新処理
/// </summary>
/// <param name="playerPos">プレイヤーの座標</param>
/// <param name="enemyPos">敵の座標</param>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------
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

// ---------------------------------------------
/// <summary>
/// ステートの後更新処理
/// </summary>
// ---------------------------------------------
void PlayCameraState::PostUpdate()
{
}