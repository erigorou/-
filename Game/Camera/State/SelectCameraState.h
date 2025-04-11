// ---------------------------------------------
// 
// 名前		:SelectCameraState.h
// 内容		:ステージ選択時のカメラステートクラス
// 作成		:池田桜輔
// 
// ---------------------------------------------
#pragma once
// インクルード
#include "pch.h"
#include "Interface/ICameraState.h"

// 前方宣言
class Camera;

class SelectCameraState : public ICameraState
{
	// ----------------------------
	// 定数
	// ----------------------------
private:
	// sin波の周期
	static constexpr float SINE_PERIOD = 4.0f;
	// 上下幅
	static constexpr float SINE_HEIGHT = 1.0f;
	// 補間率
	static constexpr float LERP_RATE = 0.075f;

	// ----------------------------
	// メンバ関数
	// ----------------------------
public:
	// コンストラクタ
	SelectCameraState(Camera* camera);
	// デストラクタ
	~SelectCameraState()override;
	// ステート変更（in）
	void PreUpdate()override;
	// 更新処理
	void Update(
		const DirectX::SimpleMath::Vector3& playerPos,
		const DirectX::SimpleMath::Vector3& enemyPos,
		float elapsedTime
	)override;
	// ステート変更（out）
	void PostUpdate()override;

	// ----------------------------
	// メンバ変数
	// ----------------------------
private:
	// カメラ
	Camera* m_camera;
	// カメラのY座標
	float m_cameraY;
	// 経過時間
	float m_elapsedTime;
};