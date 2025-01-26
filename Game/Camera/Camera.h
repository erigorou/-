#pragma once

#include "State/TitleCameraState.h"
#include "State/PlayCameraState.h"

class ICameraState;
class TitleCameraState;
class PlayCameraState;

class Camera
{

public:
	// 固定値
	static constexpr float CAMERA_POSITION_Y	= 10.0f;	// カメラのY座標
	static constexpr float CAMERA_DIRECTION		= 30.0f;	// カメラの距離
	static constexpr float TARGET_HEIGHT		= 8.0f;		// 注視点の高さ

	static constexpr float SHAKE_TIME	= 0.5f;		// カメラを揺らす時間
	static constexpr float SHAKE_POWER	= 0.5f;		// カメラのやれる強さ
	static constexpr float MINIMAL		= 0.01f;	// 極小値

	static constexpr float CAMERA_EYE_RATE		= 0.5f;	// カメラの追従係数
	static constexpr float CAMERA_TARGET_RATE	= 0.1f;	// ターゲットの追従係数

	// ステート
	TitleCameraState*	GetTitleState()	{ return m_titleState.get();	}
	PlayCameraState*	GetPlayState()	{ return m_playState.get();		}


	const DirectX::SimpleMath::Matrix&	GetViewMatrix			() const { return m_view;		}	// ビュー行列
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

	void SetShake(void* power);			// カメラを揺らす // 引数にはフロート型のポインタを渡す

	void CalculateViewMatrix();			// ビュー行列を計算する
	void CalculateCameraAngle();		// カメラのアングルを計算する

	void Shake(float elapsedTime);						// カメラを揺らす

	void ChangeState(ICameraState* state);	// ステートの変更

	DirectX::SimpleMath::Vector3	m_position;		// カメラ位置
	DirectX::SimpleMath::Matrix		m_view;			// ビュー行列
	DirectX::SimpleMath::Matrix		m_projection;	// プロジェクション行列
	DirectX::SimpleMath::Vector3	m_eye;			// カメラ座標
	DirectX::SimpleMath::Vector3	m_target;		// 注視点
	DirectX::SimpleMath::Vector3	m_up;			// カメラの頭の方向
	
	float m_currentAngle;			// 現在のアングル
	float m_angle;					// アングル
	float m_targetHeight;			// 注視点の高さ

	bool m_isShake;					// カメラを揺らすかどうか
	float m_shakeTime;				// 揺れる時間

	DirectX::SimpleMath::Vector3 m_shakePos;	// 揺れる座標
	float m_shakePower;							// 揺れる強さ

private:

	void CreateState();	// ステートの生成

	// ステート用 ///////////
	ICameraState* m_currentState;	// 現在のステート

	std::unique_ptr<TitleCameraState>	m_titleState;
	std::unique_ptr<PlayCameraState>	m_playState;



};