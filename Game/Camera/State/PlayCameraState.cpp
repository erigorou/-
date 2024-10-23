#include "pch.h"
#include "PlayCameraState.h"
#include "Interface/ICameraState.h"
#include "../Camera.h"


// コンストラクタ
PlayCameraState::PlayCameraState(Camera* camera)
	: m_camera(camera)
{
}


// デストラクタ
PlayCameraState::~PlayCameraState()
{
}


// ステート変更（in）
void PlayCameraState::PreUpdate()
{
}


// 更新処理
void PlayCameraState::Update(
	const DirectX::SimpleMath::Vector3& playerPos,
	const DirectX::SimpleMath::Vector3& enemyPos,
	const DirectX::SimpleMath::Matrix& rotate,
	float elapsedTime
)
{
	UNREFERENCED_PARAMETER(rotate);

    // カメラの通常の更新処理を実行
    using namespace DirectX::SimpleMath;

    // カメラの向きや位置を計算する処理
    Vector3 playerToEnemy = enemyPos - playerPos;
    playerToEnemy.Normalize();

    m_camera->m_position = playerPos - playerToEnemy * Camera::CAMERA_DIRECTION;
    m_camera->m_position.y = Camera::CAMERA_POSITION_Y;

    m_camera->m_target = enemyPos + playerToEnemy;

    // カメラの振動処理
    m_camera->Shake(0.016f); // 仮の経過時間

	m_camera->m_target.y = m_camera->m_targetHeight;

    // ビュー行列を更新
    m_camera->CalculateViewMatrix();
    m_camera->CalculateCameraAngle();
}


// ステート変更（out)
void PlayCameraState::PostUpdate()
{
}

