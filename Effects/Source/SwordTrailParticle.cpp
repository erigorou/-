// -------------------------------------
//
// 剣の軌跡の動きを実装
//
// -------------------------------------
#include "pch.h"
#include "Effects/Header/SwordTrailParticle.h"


// --------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="vertex">頂点情報</param>
/// <param name="life">生存時間</param>
/// <param name="startColor">初期カラー</param>
/// <param name="endColor">終了カラー</param>
// --------------------------------------------------------
SwordTrailParticle::SwordTrailParticle(
	DirectX::VertexPositionTexture vertex[],
	float life,
	DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor
)
	:m_startLife(life)			// 生存時間
	, m_life(life)				// 現在時間
	, m_startColor(startColor)	// 最初の色
	, m_nowColor(startColor)		// 現在の色
	, m_endColor(endColor)		// 終了の色
{
	// 配列の各要素を初期化
	for (int i = 0; i < 4; i++)
	{
		m_vertices[i] = vertex[i];
	}
}

// --------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// --------------------------------------------------------
SwordTrailParticle::~SwordTrailParticle()
{
}


// --------------------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
/// <returns>生存しているか</returns>
// --------------------------------------------------------
bool SwordTrailParticle::Update(float elapsedTime)
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