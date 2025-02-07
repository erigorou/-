// -------------------------------------- //
//
// 土埃エフェクトの挙動を計算するクラス
//
// -------------------------------------- //

#pragma once
#include "pch.h"
#include "Interface/IParticle.h"

class DustTrailParticle : public IParticle
{
	// ----------------------------
	// アクセサ
	// ----------------------------
public:
	// 座標
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }
	// 速度
	DirectX::SimpleMath::Vector3 GetVelocity() { return m_velocity; }
	// 加速度
	DirectX::SimpleMath::Vector3 GetAccelerator() { return m_accelerator; }
	// 現在のサイズ
	DirectX::SimpleMath::Vector3 GetNowScale() { return m_nowScale; }
	// 開始時サイズ
	DirectX::SimpleMath::Vector3 GetStartScale() { return m_startScale; }
	// 終了時サイズ
	DirectX::SimpleMath::Vector3 GetEndScale() { return m_endScale; }
	// 表示時間
	float GetLife() { return m_life; }
	// 開始時間
	float GetStartLife() { return m_startLife; }
	// 現在の色
	DirectX::SimpleMath::Color GetNowColor() { return m_nowColor; }
	// 開始時色
	DirectX::SimpleMath::Color GetStartColor() { return m_startColor; }
	// 終了時色
	DirectX::SimpleMath::Color GetEndColor() { return m_endColor; }

	// ----------------------------
	// メンバ関数（公開）
	// ----------------------------
public:

	// コンストラクタ
	DustTrailParticle(
		float life,
		DirectX::SimpleMath::Vector3 pos,													// 初期座標
		DirectX::SimpleMath::Vector3 velocity,												// 初期速度
		DirectX::SimpleMath::Vector3 accelerator,											// 加速度
		DirectX::SimpleMath::Vector3 startScale, DirectX::SimpleMath::Vector3 endScale,		// 初期サイズと最終サイズ
		DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor			// 初期カラーと最終カラー
	);
	// デストラクタ
	~DustTrailParticle();
	// 更新処理
	bool Update(float elapsedTime);

	// ----------------------------
	// メンバ変数
	// ----------------------------
private:
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
};