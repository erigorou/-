// -----------------------------
// カメラを追加
// -----------------------------

#include "pch.h"
#include "Game/Camera/Camera.h"
#include "Game/Screen.h"

// 固定値
const float Camera::CAMERA_POSITION_Y = 5.0f;		// カメラの高さ
const float Camera::CAMERA_DIRECTION = 20.0f;		// カメラの距離

//-------------------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="target">注視点</param>
//-------------------------------------------------------------------
Camera::Camera(const DirectX::SimpleMath::Vector3& target)
	:m_view{}
	,m_projection{}
	,m_eye{}
	,m_target{ target }
	,m_up{ DirectX::SimpleMath::Vector3::UnitY }
	,m_position{}
	,m_angle{}
	,m_targetHeight{5.0f}
{
}

//-------------------------------------------------------------------
/// <summary>
/// カメラの更新処理
/// </summary>
/// <param name="playerPos">プレイヤーの座標</param>
/// <param name="enemyPos">注視する座標</param>
/// <param name="rotate">プレイヤーの回転量/param>
//-------------------------------------------------------------------
void Camera::Update(
	const DirectX::SimpleMath::Vector3& playerPos,
	const DirectX::SimpleMath::Vector3& enemyPos,
	const DirectX::SimpleMath::Matrix& rotate
)
{	using namespace DirectX;

	UNREFERENCED_PARAMETER(rotate);

	// プレイヤーからターゲットへのベクトルを計算
	SimpleMath::Vector3 playerToEnemy = enemyPos - playerPos;
	// ベクトルを正規化して単位ベクトルを取得
	SimpleMath::Vector3 unitVecPlayerToTarget = playerToEnemy;
	unitVecPlayerToTarget.Normalize();
	// カメラ位置を計算
	m_position = playerPos - unitVecPlayerToTarget * CAMERA_DIRECTION;
	m_position.y = CAMERA_POSITION_Y; // Y座標を固定
	// 注視点を計算
	m_target = enemyPos + unitVecPlayerToTarget;
	// 注視点の高さを設定
	m_target.y = m_targetHeight;
	// ビュー行列を更新する
	CalculateViewMatrix();
	// カメラのアングルを計算する
	CalculateCameraAngle();
}

//-------------------------------------------------------------------
/// <summary>
/// カメラのビュー行列を
/// </summary>
//-------------------------------------------------------------------
void Camera::CalculateViewMatrix()
{
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_position, m_target, m_up);
}

//-------------------------------------------------------------------
/// <summary>
/// カメラのアングルを計算する
/// </summary>
//-------------------------------------------------------------------
void Camera::CalculateCameraAngle()
{
	using namespace DirectX::SimpleMath;

	// カメラの前方向ベクトル
	Vector3 forward = m_target - m_position;
	forward.Normalize();

	// 世界座標系の前方向ベクトル
	Vector3 worldForward = Vector3::Forward;

	// 内積を計算
	float dotProduct = forward.Dot(worldForward);

	// 内積から角度を計算（弧度法）
	m_angle = acosf(dotProduct);

	// カメラの前方向ベクトルが右方向に向いているかどうかで符号を決定
	Vector3 crossProduct = forward.Cross(worldForward);
	if (crossProduct.y < 0)
	{
		m_angle = -m_angle;
	}
}