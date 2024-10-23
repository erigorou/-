// -----------------------------
// カメラを追加
// -----------------------------

#include "pch.h"
#include "Game/Camera/Camera.h"
#include "Game/Screen.h"
#include "Libraries/MyLib/Math.h"

#include "Interface/ICameraState.h"
#include "State/TitleCameraState.h"
#include "State/PlayCameraState.h"

// 固定値
const float Camera::CAMERA_POSITION_Y = 8.0f;		// カメラの高さ
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
	, m_isShake{ false }
	, m_shakeTime{ SHAKE_TIME }
{
	CreateState();
}

//-------------------------------------------------------------------
/// <summary>
/// カメラの更新処理
/// </summary>
/// <param name="playerPos">プレイヤーの座標</param>
/// <param name="enemyPos">注視する座標</param>
/// <param name="rotate">プレイヤーの回転量/param>
/// <param name="elapsedTime">経過時間</param>
//-------------------------------------------------------------------
void Camera::Update(
	const DirectX::SimpleMath::Vector3& playerPos,
	const DirectX::SimpleMath::Vector3& enemyPos,
	const DirectX::SimpleMath::Matrix&	rotate,
	const float							elapsedTime
)
{
	// ステートを更新
	m_currentState->Update(
		playerPos,
		enemyPos,
		rotate,
		elapsedTime
	);

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


// -----------------------------
// カメラの振動
// -----------------------------
void Camera::Shake(float elapsedTime)
{
	using namespace DirectX;

	if ( ! m_isShake ) return;

	m_shakeTime -= elapsedTime;

	// 振動時間が0以下になったら振動を終了
	if (m_shakeTime <= 0.0f)
	{
		m_isShake = false;  
		return;
	}

	float power = m_shakeTime * 1.0f;

	SimpleMath::Vector3 max = SimpleMath::Vector3(power, power, power);
	SimpleMath::Vector3 min = SimpleMath::Vector3(-power, -power, -power);

	// カメラの位置を揺らす
	m_position	+=	Math::RandomVector3(min, max);
	m_target	+=	Math::RandomVector3(min, max);
}


//-------------------------------------------------------------------
// ステートの変更
//-------------------------------------------------------------------
void Camera::ChangeState(ICameraState* state)
{
	// ステートが同じなら変更しない
	if (m_currentState == state) return;

	// ステートの変更手続き
	m_currentState->PostUpdate();
	m_currentState = state;
	m_currentState->PreUpdate();
}



//-------------------------------------------------------------------
// カメラの振動を開始
//-------------------------------------------------------------------
void Camera::CreateState()
{
	m_titleState	= std::make_unique<TitleCameraState>(this);
	m_playState		= std::make_unique<PlayCameraState>(this);


	m_currentState = m_titleState.get();
}


//-------------------------------------------------------------------
// カメラの振動を開始
//-------------------------------------------------------------------
void Camera::SetShake()
{
	m_isShake = true;
	m_shakeTime = SHAKE_TIME;
}