// -------------------------------------
//
// 移動したら発生する土埃の動きを実装
//
// -------------------------------------
#include "pch.h"
#include "Interface/IParticle.h"
#include "Effects/Header/DustTrailParticle.h"

// 終了時間
const static float ENDLESS = -100.0f;

// ----------------------------
// コンストラクタ
// ----------------------------
DustTrailParticle::DustTrailParticle(
	float life,
	DirectX::SimpleMath::Vector3 pos,
	DirectX::SimpleMath::Vector3 velocity,
	DirectX::SimpleMath::Vector3 accelerator,
	DirectX::SimpleMath::Vector3 startScale, DirectX::SimpleMath::Vector3 endScale,
	DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor
)
	:
	m_startLife(life),			// 生存時間
	m_life(life),				// 現在時間
	m_position(pos),			// 座標
	m_velocity(velocity),		// 速度
	m_accelerator(accelerator),	// 加速度ベクトル
	m_startScale(startScale),	// 最初のサイズ
	m_nowScale(startScale),		// 現在のサイズ
	m_endScale(endScale),		// 終了のサイズ
	m_startColor(startColor),	// 最初の色
	m_nowColor(startColor),		// 現在の色
	m_endColor(endColor)		// 終了の色
{
}

// ----------------------------
// デストラクタ
// ----------------------------
DustTrailParticle::~DustTrailParticle()
{
}

// ----------------------------
// 更新処理
// ----------------------------
bool DustTrailParticle::Update(float elapsedTime)
{
	//	スケール。現在の生存時間から、大きさをLerpで算出する
	m_nowScale = DirectX::SimpleMath::Vector3::Lerp(m_startScale, m_endScale, 1.0f - m_life / m_startLife);

	// 色。現在の生存時間から、色をLerpで算出する
	m_nowColor = DirectX::SimpleMath::Color::Lerp(
		m_startColor,
		DirectX::SimpleMath::Color(m_endColor.R(), m_endColor.G(), m_endColor.B(), 0), // 透明度を0に設定して透明にする
		1.0f - m_life / m_startLife
	);

	//	速度の計算。経過時間と加速度を掛け算して足すことで、加速度を考慮して移動速度を変化させていく
	m_velocity += m_accelerator * elapsedTime;
	//	座標の計算。経過時間と移動速度から、現在の座標を算出する
	m_position += m_velocity * elapsedTime;
	//	ライフを減らしていく
	m_life -= elapsedTime;
	//	ライフが0未満なら自身を消してもらう
	if (m_life < 0.0f)
	{
		return false;
	}

	return true;
}