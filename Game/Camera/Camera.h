#pragma once

class Camera
{
private:
	// 固定値
	static const float CAMERA_POSITION_Y;		// カメラのY座標
	static const float CAMERA_DIRECTION;		// カメラの距離

	// カメラの座標
	DirectX::SimpleMath::Vector3 m_position;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// プロジェクション行列
	DirectX::SimpleMath::Matrix m_projection;
	// カメラ座標
	DirectX::SimpleMath::Vector3 m_eye;
	// 注視点
	DirectX::SimpleMath::Vector3 m_target;
	// カメラの頭の方向
	DirectX::SimpleMath::Vector3 m_up;
	// アングル
	float m_angle;
	// 注視点の高さ
	float m_targetHeight;


public:
	// コンストラクタ
	Camera(const DirectX::SimpleMath::Vector3& target = DirectX::SimpleMath::Vector3::Zero);
	// デストラクタ
	~Camera() = default;

	// 更新処理
	void Update(
		const DirectX::SimpleMath::Vector3& playerPos,
		const DirectX::SimpleMath::Vector3& enemyPos,
		const DirectX::SimpleMath::Matrix& rotate
	);


	// getter
	// ビュー行列
	const DirectX::SimpleMath::Matrix& GetViewMatrix() const { return m_view; }
	// プロジェクション
	const DirectX::SimpleMath::Matrix& GetProjectionMatrix() const { return m_projection; }
	// カメラ座標
	const DirectX::SimpleMath::Vector3& GetEyePosition() const { return m_position; }
	// 注視点
	const DirectX::SimpleMath::Vector3& GetTargetPosition() const { return m_target; }
	// 向いている方向
	const DirectX::SimpleMath::Vector3& GetUpVector() const { return m_up; }
	// カメラのアングル
	const float& GetCameraAngle() const { return m_angle; }


private:
	// ビュー行列を計算する
	void CalculateViewMatrix();

	// カメラのアングルを計算する
	void CalculateCameraAngle();
};