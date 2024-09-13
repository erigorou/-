// -------------------------------------
//
// 剣の軌跡の動きを実装
//
// -------------------------------------
#include "pch.h"
#include "Interface/IParticle.h"
#include "Effects/Header/SwordTrialParticle.h"


/// <summary>
/// コンストラクタ
/// 引数は初期設定用の数値
/// </summary>

SwordTrialParticle::SwordTrialParticle(
	DirectX::VertexPositionTexture vertex[4],
	float life,
	DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor
	)
	:m_startLife(life)			// 生存時間
	,m_life(life)				// 現在時間
	,m_startColor(startColor)	// 最初の色
	,m_nowColor(startColor)		// 現在の色
	,m_endColor(endColor)		// 終了の色
{
	// 配列の各要素を初期化
	for (int i = 0; i < 3; i++)
	{
		m_vertices[i] = vertex[i];
	}
}


/// <summary>
/// デストラクタ
/// </summary>
SwordTrialParticle::~SwordTrialParticle()
{
}


/// <summary>
/// 更新関数
/// </summary>
/// <param name="elapsedTimer">Game等から受け取る</param>
/// <returns>生存時間(life)がある間はtrue</returns>
bool SwordTrialParticle::Update(float elapsedTime)
{
	using namespace DirectX;

	// 色。現在の生存時間から、色をLerpで算出する
	m_nowColor = SimpleMath::Color::Lerp(
		m_startColor,
		DirectX::SimpleMath::Color(m_endColor.R(), m_endColor.G(), m_endColor.B(), 0), // 透明度を0に設定して透明にする
		1.0f - m_life / m_startLife);

	//	ライフを減らしていく
	m_life -= elapsedTime;
	//	ライフが0未満なら自身を消してもらう
	if (m_life < 0.0f)return false;

	return true;
}