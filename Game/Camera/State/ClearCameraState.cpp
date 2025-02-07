// ---------------------------------------------
// 名前:	ClearCameraState.cpp
// 内容:	クリア時のカメラステートクラス
// 作成:	池田桜輔
// ---------------------------------------------
// インクルード
#include "pch.h"
#include "ClearCameraState.h"
#include "../Camera.h"
#include "Libraries/MyLib/Math.h"

// ---------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="camera">カメラオブジェクト</param>
// ---------------------------------------------
ClearCameraState::ClearCameraState(Camera* camera)
	: 
	m_camera(camera),
	m_totalTime{}
{
}


// ---------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------------
ClearCameraState::~ClearCameraState()
{
}

// ---------------------------------------------
/// <summary>
/// ステートの前更新処理
/// </summary>
// ---------------------------------------------
void ClearCameraState::PreUpdate()
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
void ClearCameraState::Update(
	const DirectX::SimpleMath::Vector3& playerPos,
	const DirectX::SimpleMath::Vector3& enemyPos,
	float elapsedTime
)
{
	// 時間を加算
	m_totalTime += elapsedTime;

	// 徐々に遠ざかる距離を計算（Lerp）
	float distanceFactor = std::min(m_totalTime / MAX_TIME, 1.0f);
	float currentDistance = START_DISTANCE + (MAX_DISTANCE - START_DISTANCE) * distanceFactor;

	// カメラの角度を計算（円運動）
	float angle = ROTATION_SPEED * m_totalTime;
	float offsetX = cosf(angle) * currentDistance;
	float offsetZ = sinf(angle) * currentDistance;

	// カメラの高さ（ゆっくり上昇）
	float heightOffset = CAMERA_HEIGHT + (CAMERA_HEIGHT * distanceFactor);

	// 敵を中心にカメラを配置
	Vector3 targetCameraPos = Vector3(offsetX, heightOffset, offsetZ);

	// カメラ位置を補間して追従
	m_camera->m_position = Math::LerpVector(m_camera->m_position, targetCameraPos, 0.02f);

	// 注視点を敵の座標に固定
	m_camera->m_target = Math::LerpVector(m_camera->m_target, enemyPos, 0.05f);

	// ビュー行列の更新
	m_camera->CalculateViewMatrix();
}



// ---------------------------------------------
/// <summary>
/// ステートの後更新処理
/// </summary>
// ---------------------------------------------
void ClearCameraState::PostUpdate()
{
}