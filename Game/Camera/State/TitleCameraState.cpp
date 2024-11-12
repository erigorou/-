#include "pch.h"

#include "TitleCameraState.h"
#include "../Camera.h"

// コンストラクタ
TitleCameraState::TitleCameraState(Camera* camera)
	: m_camera(camera)
	, m_angle(0.0f)
{
}

// デストラクタ
TitleCameraState::~TitleCameraState()
{
}


// ステート変更（in）
void TitleCameraState::PreUpdate()
{
}


// 更新処理
void TitleCameraState::Update(
    const DirectX::SimpleMath::Vector3& playerPos,
    const DirectX::SimpleMath::Vector3& enemyPos,
    const DirectX::SimpleMath::Matrix& rotate,
    float elapsedTime
)
{
    UNREFERENCED_PARAMETER(playerPos);
    UNREFERENCED_PARAMETER(enemyPos);
    UNREFERENCED_PARAMETER(rotate);

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


// ステート変更（out)
void TitleCameraState::PostUpdate()
{
}
