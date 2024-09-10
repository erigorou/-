// ----------------
//
// ゲージ
//
// ----------------

#include "pch.h"
#include "Game/UI/Header/Gauge.h"
#include "Interface/IUserInterface.h"


// ----------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// ---------------------------
GaugeUI::GaugeUI()
	:m_spriteBatch()
	,m_texture()
	,m_position()
	,m_size()
{
}



// ----------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ----------------------------
GaugeUI::~GaugeUI()
{}



// ----------------------------
/// <summary>
/// 初期化処理
/// </summary>
/// <param name="position">座標　（anchor = mid）</param>
/// <param name="size"></param>
// ----------------------------
void GaugeUI::Initialize(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 size)
{
	m_position	= position;
	m_size		= size;
}



// ----------------------------

/// <summary>
/// 更新処理
/// </summary>

// ----------------------------
void GaugeUI::Update()
{
}



// ----------------------------
/// <summary>
/// 描画関数
/// </summary>
// ----------------------------
void GaugeUI::Render()
{
}



// ----------------------------
/// <summary>
/// 終了関数
/// </summary>
// ----------------------------
void GaugeUI::Finalize()
{
}
