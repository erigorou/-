#pragma once

#include "Interface/ICameraState.h"
#include "State/TitleCameraState.h"
#include "State/PlayCameraState.h"
#include "State/ClearCameraState.h"

enum class CameraState
{
	Title,
	Play,
	Clear
};

class Camera
{
	// ----------------------------
	// 定数
	// ----------------------------
public:
	// カメラの初期位置
	static constexpr float CAMERA_POSITION_Y = 10.0f;
	// カメラの初期角度
	static constexpr float CAMERA_DIRECTION = 30.0f;
	// ターゲットの高さ
	static constexpr float TARGET_HEIGHT = 8.0f;

	// カメラの振動
	static constexpr float SHAKE_TIME = 0.5f;
	// カメラの振動の強さ
	static constexpr float SHAKE_POWER = 0.5f;
	// 極小値
	static constexpr float MINIMAL = 0.01f;

	// カメラの追従係数
	static constexpr float CAMERA_EYE_RATE = 0.5f;
	// ターゲットの追従係数
	static constexpr float CAMERA_TARGET_RATE = 0.05f;

	// ----------------------------
	// アクセサ
	// ----------------------------
public:
	// ビュー行列
	DirectX::SimpleMath::Matrix GetViewMatrix() const { return m_view; }
	// プロジェクション行列
	DirectX::SimpleMath::Matrix GetProjectionMatrix() const { return m_projection; }
	// カメラ座標
	DirectX::SimpleMath::Vector3 GetEyePosition() const { return m_position; }
	// 注視点
	DirectX::SimpleMath::Vector3 GetTargetPosition() const { return m_target; }
	// カメラの頭の方向
	DirectX::SimpleMath::Vector3 GetUpVector() const { return m_up; }
	// アングル
	float GetCameraAngle() const { return m_angle; }

	// ----------------------------
	// メンバ関数(公開)
	// ----------------------------
public:
	// コンストラクタ
	Camera(const DirectX::SimpleMath::Vector3& target = DirectX::SimpleMath::Vector3::Zero);
	// デストラクタ
	~Camera() = default;
	// 更新処理
	void Update(
		const DirectX::SimpleMath::Vector3& playerPos,
		const DirectX::SimpleMath::Vector3& enemyPos,
		const float elapsedTime
	);
	// カメラを揺らす
	void SetShake(void* power);
	// ビュー行列の計算
	void CalculateViewMatrix();
	// カメラのアングルを計算
	void CalculateCameraAngle();
	// カメラを揺らす
	void Shake(float elapsedTime);
	// ステートの変更
	void ChangeState(void* state);

	// ----------------------------
	// メンバ関数(非公開)
	// ----------------------------
private:
	// ステートの作成
	void CreateState();

	// ----------------------------
	// メンバ変数
	// ----------------------------
public:
	// カメラの位置
	DirectX::SimpleMath::Vector3 m_position;
	// ビュー行列
	DirectX::SimpleMath::Matrix	 m_view;
	// プロジェクション行列
	DirectX::SimpleMath::Matrix m_projection;
	// カメラの注視点
	DirectX::SimpleMath::Vector3 m_target;
	// カメラの頭の方向
	DirectX::SimpleMath::Vector3 m_up;
	// カメラのアングル
	float m_currentAngle;
	// カメラのアングル
	float m_angle;
	// ターゲットの高さ
	float m_targetHeight;
	// カメラの振動
	bool m_isShake;
	// 振動時間
	float m_shakeTime;
	// カメラの振動位置
	DirectX::SimpleMath::Vector3 m_shakePos;
	// カメラの振動の強さ
	float m_shakePower;
	// 現在のステート
	ICameraState* m_currentState;
	// タイトルステート
	std::unique_ptr<TitleCameraState> m_titleState;
	// プレイステート
	std::unique_ptr<PlayCameraState> m_playState;
	// クリアステート
	std::unique_ptr<ClearCameraState> m_clearState;

	// ステートを格納
	std::vector<ICameraState*> m_states;
};