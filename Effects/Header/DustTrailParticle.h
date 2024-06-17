// -------------------------------------------------------------------
//
// 動くものの後ろに付くパーティクルの動き
//
// -------------------------------------------------------------------

#pragma once
#include "pch.h"

class DustTrialParticle
{
public:
	// ゲッター************************************
	const DirectX::SimpleMath::Vector3 GetPosition		() { return m_position; }		// 座標
	const DirectX::SimpleMath::Vector3 GetVelocity		() { return m_velocity; }		// 速度
	const DirectX::SimpleMath::Vector3 GetAccelerator	() { return m_accelerator; }	// 加速度
	const DirectX::SimpleMath::Vector3 GetNowScale	() { return m_nowScale; }		// 現在のサイズ
	const DirectX::SimpleMath::Vector3 GetStartScale() { return m_startScale; }		// 開始時サイズ
	const DirectX::SimpleMath::Vector3 GetEndScale	() { return m_endScale; }		// 終了時サイズ
	const float GetLife		() { return m_life; }			// 表示時間
	const float GetStartLife() { return m_startLife; }		// 開始時間
	const DirectX::SimpleMath::Color GetNowColor	() { return m_nowColor; }		// 現在の色
	const DirectX::SimpleMath::Color GetStartColor	() { return m_startColor; }		// 開始時色
	const DirectX::SimpleMath::Color GetEndColor	() { return m_endColor; }		// 終了時色
	// ********************************************

private:
	// 変数宣言
	// 土埃の情報 *********************************
	// 座標
	DirectX::SimpleMath::Vector3 m_position;
	// 速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// 加速度ベクトル
	DirectX::SimpleMath::Vector3 m_accelerator;
	// スケール
	DirectX::SimpleMath::Vector3 m_nowScale;
	DirectX::SimpleMath::Vector3 m_startScale;
	DirectX::SimpleMath::Vector3 m_endScale;
	//	生存時間
	float m_life;
	float m_startLife;
	//	カラー
	DirectX::SimpleMath::Color m_nowColor;
	DirectX::SimpleMath::Color m_startColor;
	DirectX::SimpleMath::Color m_endColor;
	// *******************************************
public:

	// 関数宣言
	// コンストラクタ
	DustTrialParticle(
		float life,
		DirectX::SimpleMath::Vector3 pos,													// 初期座標
		DirectX::SimpleMath::Vector3 velocity,												// 初期速度
		DirectX::SimpleMath::Vector3 accelerator,												// 加速度
		DirectX::SimpleMath::Vector3 startScale, DirectX::SimpleMath::Vector3 endScale,		// 初期サイズと最終サイズ
		DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor			// 初期カラーと最終カラー
	);
	// デストラクタ
	~DustTrialParticle();
	// 更新処理
	bool Update(float elapsedTime);
};