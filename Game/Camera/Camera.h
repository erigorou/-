#pragma once

class ICameraState;

class Camera
{
	class CameraIdling;


public:
	// 固定値
	static const float CAMERA_POSITION_Y;			// カメラのY座標
	static const float CAMERA_DIRECTION;			// カメラの距離
	static constexpr float SHAKE_TIME = 0.5f;		// カメラを揺らす時間
	static constexpr float MINIMAL = 0.01f;	// 最小値

	const DirectX::SimpleMath::Matrix&		GetViewMatrix		() const { return m_view;		}	// ビュー行列
	const DirectX::SimpleMath::Matrix&	GetProjectionMatrix		() const { return m_projection; }	// プロジェクション行列
	const DirectX::SimpleMath::Vector3& GetEyePosition			() const { return m_position;	}	// カメラ座標
	const DirectX::SimpleMath::Vector3& GetTargetPosition		() const { return m_target;		}	// 注視点
	const DirectX::SimpleMath::Vector3& GetUpVector				() const { return m_up;			}	// カメラの頭の方向
	const float&						GetCameraAngle			() const { return m_angle;		}	// アングル



	Camera(const DirectX::SimpleMath::Vector3& target = DirectX::SimpleMath::Vector3::Zero);	// コンストラクタ
	~Camera() = default;																		// デストラクタ	


	// 更新処理
	void Update(
		const DirectX::SimpleMath::Vector3& playerPos,
		const DirectX::SimpleMath::Vector3& enemyPos,
		const DirectX::SimpleMath::Matrix& rotate,
		const float elapsedTime
	);

	void SetShake();	// カメラを揺らす

private:

	void CalculateViewMatrix();			// ビュー行列を計算する
	void CalculateCameraAngle();		// カメラのアングルを計算する

	void Shake(float elapsedTime);						// カメラを揺らす

	DirectX::SimpleMath::Vector3	m_position;		// カメラ位置
	DirectX::SimpleMath::Matrix		m_view;			// ビュー行列
	DirectX::SimpleMath::Matrix		m_projection;	// プロジェクション行列
	DirectX::SimpleMath::Vector3	m_eye;			// カメラ座標
	DirectX::SimpleMath::Vector3	m_target;		// 注視点
	DirectX::SimpleMath::Vector3	m_up;			// カメラの頭の方向
	

	float m_angle;					// アングル
	float m_targetHeight;			// 注視点の高さ

	bool m_isShake;					// カメラを揺らすかどうか
	float m_shakeTime;				// 揺れる時間

	// ステート用 ///////////
	ICameraState* m_currentState;	// 現在のステート

	

};