// -----------------------------
// カメラを追加
// -----------------------------

#include "pch.h"
#include "Game/Camera/Camera.h"
#include "Game/Screen.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Messenger/EventMessenger.h"

#include "Interface/ICameraState.h"
#include "State/TitleCameraState.h"
#include "State/PlayCameraState.h"

//-------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------
Camera::Camera(const DirectX::SimpleMath::Vector3& target)
	:m_view{}
	,m_projection{}
	,m_target{ target }
	,m_up{ DirectX::SimpleMath::Vector3::UnitY }
	,m_position{}
	,m_angle{}
	,m_targetHeight{TARGET_HEIGHT}
	, m_isShake{ false }
	, m_shakeTime{ SHAKE_TIME }
	, m_shakePos{}
{
	// ステートを作成
	CreateState();

	// カメラのシェイクをイベントに登録
	EventMessenger::Attach(EventList::ShakeCamera, std::bind(&Camera::SetShake, this, std::placeholders::_1));
}

//-------------------------------------------------------------------
// カメラの更新処理
//-------------------------------------------------------------------
void Camera::Update(
	const DirectX::SimpleMath::Vector3& playerPos,
	const DirectX::SimpleMath::Vector3& enemyPos,
	const float	elapsedTime
)
{
	// ステートを更新
	m_currentState->Update(
		playerPos,
		enemyPos,
		elapsedTime
	);
}

//-------------------------------------------------------------------
// カメラのビュー行列を計算する
//-------------------------------------------------------------------
void Camera::CalculateViewMatrix()
{
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_position, m_target, m_up);
}


//-------------------------------------------------------------------
// カメラのアングルを計算する
//-------------------------------------------------------------------
void Camera::CalculateCameraAngle()
{
	// カメラの前方向ベクトル
	DirectX::SimpleMath::Vector3 forward = m_target - m_position;
	forward.Normalize();

	// 世界座標系の前方向ベクトル
	DirectX::SimpleMath::Vector3 worldForward = DirectX::SimpleMath::Vector3::Forward;

	// 内積を計算
	float dotProduct = forward.Dot(worldForward);

	// 内積から角度を計算（弧度法）
	float targetAngle = acosf(dotProduct);

	// カメラの前方向ベクトルが右方向に向いているかどうかで符号を決定
	DirectX::SimpleMath::Vector3 crossProduct = forward.Cross(worldForward);
	if (crossProduct.y < 0)
	{
		targetAngle = -targetAngle;
	}

	// 線形補間で現在のアングルを更新
	m_currentAngle = Math::LerpFloat(m_currentAngle, targetAngle, CAMERA_TARGET_RATE);

	// 更新されたアングルを反映
	m_angle = m_currentAngle;
}


// -----------------------------
// カメラの振動
// -----------------------------
void Camera::Shake(float elapsedTime)
{
	if ( ! m_isShake ) return;

	m_shakeTime -= elapsedTime;

	// 振動時間が0以下になったら振動を終了
	if (m_shakeTime <= 0.0f)
	{
		m_isShake = false;  
		return;
	}

	// shakeTimeに応じて振動の強さを決定
	float power = (m_shakeTime / SHAKE_TIME) * m_shakePower;

	DirectX::SimpleMath::Vector3 max = DirectX::SimpleMath::Vector3(power	, power	, power	);
	DirectX::SimpleMath::Vector3 min = DirectX::SimpleMath::Vector3(-power, -power, -power);

	// カメラの位置を揺らす
	m_shakePos	=	Math::RandomVector3(min, max);
	m_target   +=	Math::RandomVector3(min, max);
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
void Camera::SetShake(void* power)
{
	m_isShake = true;
	m_shakeTime = SHAKE_TIME;
	m_shakePower = *static_cast<float*>(power);
}