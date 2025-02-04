// -----------------------------
// カメラを追加
// -----------------------------

#include "pch.h"
#include "Game/Camera/Camera.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Messenger/EventMessenger.h"

// ---------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="target">ターゲット座標</param>
// ---------------------------------------------
Camera::Camera(const DirectX::SimpleMath::Vector3& target)
	:m_view{}
	, m_projection{}
	, m_target{ target }
	, m_up{ DirectX::SimpleMath::Vector3::UnitY }
	, m_position{}
	, m_angle{}
	, m_targetHeight{ TARGET_HEIGHT }
	, m_isShake{ false }
	, m_shakeTime{ SHAKE_TIME }
	, m_shakePos{}
{
	// ステートを作成
	CreateState();

	// カメラのシェイクをイベントに登録
	EventMessenger::Attach(EventList::ShakeCamera, std::bind(&Camera::SetShake, this, std::placeholders::_1));
}

// ---------------------------------------------
/// <summary>
/// カメラの更新処理
/// </summary>
/// <param name="playerPos">プレイヤーの座標</param>
/// <param name="enemyPos">敵の座標</param>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------
void Camera::Update(
	const DirectX::SimpleMath::Vector3& playerPos,
	const DirectX::SimpleMath::Vector3& enemyPos,
	const float elapsedTime
)
{
	// ステートを更新
	m_currentState->Update(
		playerPos,
		enemyPos,
		elapsedTime
	);
}

// ---------------------------------------------
/// <summary>
/// カメラのビュー行列を計算する
/// </summary>
// ---------------------------------------------
void Camera::CalculateViewMatrix()
{
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_position, m_target, m_up);
}

// ---------------------------------------------
/// <summary>
/// カメラのアングルを計算する
/// </summary>
// ---------------------------------------------
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

// ---------------------------------------------
/// <summary>
/// カメラの振動
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------
void Camera::Shake(float elapsedTime)
{
	if (!m_isShake) return;

	m_shakeTime -= elapsedTime;

	// 振動時間が0以下になったら振動を終了
	if (m_shakeTime <= 0.0f)
	{
		m_isShake = false;
		return;
	}

	// shakeTimeに応じて振動の強さを決定
	float power = (m_shakeTime / SHAKE_TIME) * m_shakePower;

	DirectX::SimpleMath::Vector3 max = DirectX::SimpleMath::Vector3(power, power, power);
	DirectX::SimpleMath::Vector3 min = DirectX::SimpleMath::Vector3(-power, -power, -power);

	// カメラの位置を揺らす
	m_shakePos = Math::RandomVector3(min, max);
	m_target += Math::RandomVector3(min, max);
}

// ---------------------------------------------
/// <summary>
/// ステートの変更
/// </summary>
/// <param name="state">新しいカメラステート</param>
// ---------------------------------------------
void Camera::ChangeState(CameraState state)
{
	int index = static_cast<int>(state);

	// ステートが同じなら変更しない
	if (m_currentState == m_states[index]) return;

	// ステートの変更手続き
	m_currentState->PostUpdate();
	m_currentState = m_states[index];
	m_currentState->PreUpdate();
}

// ---------------------------------------------
/// <summary>
/// ステートの作成
/// </summary>
// ---------------------------------------------
void Camera::CreateState()
{
	// タイトルステートの作成
	m_titleState = std::make_unique<TitleCameraState>(this);
	m_states.push_back(m_titleState.get());

	// プレイステートの作成
	m_playState = std::make_unique<PlayCameraState>(this);
	m_states.push_back(m_playState.get());

	// 初期ステートを設定
	m_currentState = m_titleState.get();
}

// ---------------------------------------------
/// <summary>
/// カメラの振動を開始
/// </summary>
/// <param name="power">振動の強さ</param>
// ---------------------------------------------
void Camera::SetShake(void* power)
{
	m_isShake = true;
	m_shakeTime = SHAKE_TIME;
	m_shakePower = *static_cast<float*>(power);
}