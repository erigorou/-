// ---------------------------------------------
// 名前:	TitleCameraState.cpp
// 内容:	タイトル時のカメラステートクラス
// 作成:	池田桜輔
// ---------------------------------------------
// インクルード
#include "pch.h"
#include "TitleCameraState.h"
#include "../Camera.h"

// ---------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="camera">カメラオブジェクト</param>
// ---------------------------------------------
TitleCameraState::TitleCameraState(Camera* camera)
	: m_camera(camera)
	, m_angle{}
{
}

// ---------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------------
TitleCameraState::~TitleCameraState()
{
}

// ---------------------------------------------
/// <summary>
/// ステートの前更新処理
/// </summary>
// ---------------------------------------------
void TitleCameraState::PreUpdate()
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
void TitleCameraState::Update(
	const DirectX::SimpleMath::Vector3& playerPos,
	const DirectX::SimpleMath::Vector3& enemyPos,
	float elapsedTime
)
{
	UNREFERENCED_PARAMETER(playerPos);
	UNREFERENCED_PARAMETER(enemyPos);

	// 回転
	m_angle += ROTATE_SPEED * elapsedTime; // 時間に応じて角度を更新
	// カメラの位置を円運動に基づいて計算
	m_camera->m_position.x = TITLE_DIRECTION.x * cosf(m_angle) + m_camera->m_shakePos.x; // x座標
	m_camera->m_position.z = TITLE_DIRECTION.z * sinf(m_angle) + m_camera->m_shakePos.z; // z座標
	m_camera->m_position.y = TITLE_DIRECTION.y + m_camera->m_shakePos.y;                 // 高さを固定
	// カメラの注視点を中心に設定
	m_camera->m_target = DirectX::SimpleMath::Vector3::Zero + CAMERA_Y_PLUS + m_camera->m_shakePos;

	// ビュー行列を更新
	m_camera->CalculateViewMatrix();
	m_camera->CalculateCameraAngle();
}

// ---------------------------------------------
/// <summary>
/// ステートの後更新処理
/// </summary>
// ---------------------------------------------
void TitleCameraState::PostUpdate()
{
}