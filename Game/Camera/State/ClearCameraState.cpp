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

	// カメラの回転速度（ゆっくり）
	const float rotationSpeed = 0.2f;

	// 最大時間（演出時間）
	const float maxTime = 3.0f;

	// 最初のカメラ距離（開始時の距離）
	const float startDistance = 10.0f;
	// 最大カメラ距離（ズームアウト時の距離）
	const float maxDistance = 20.0f;

	// 徐々に遠ざかる距離を計算（Lerp）
	float distanceFactor = std::min(m_totalTime / maxTime, 1.0f);
	float currentDistance = startDistance + (maxDistance - startDistance) * distanceFactor;

	// カメラの角度を計算（円運動）
	float angle = rotationSpeed * m_totalTime;
	float offsetX = cosf(angle) * currentDistance;
	float offsetZ = sinf(angle) * currentDistance;

	// カメラの高さ（ゆっくり上昇）
	float heightOffset = 10.0f + (10.0f * distanceFactor);

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