// --------------
//
// EnemyHPUI.cpp
//
// --------------

#include "pch.h"
#include "Game/UI/EnemyHP/EnemyHPUI.h"
#include "Game/Data/HPSystem.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "Game/GameResources.h"

// ----------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="hpSystem">HPを所持するクラスのポインタ</param>
// ----------------------------
EnemyHPUI::EnemyHPUI(HPSystem* hpSystem)
	: m_enemyHPclass(hpSystem)
	, m_spriteBatch()
	, m_enemyHP(0)
{
}

// ----------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ----------------------------
EnemyHPUI::~EnemyHPUI()
{
}

void EnemyHPUI::Initialize()
{
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// スプライトバッチの設定
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

	// テクスチャの読み込み
	m_texture = GameResources::GetInstance()->GetTexture("enemyHP");
	m_backTexture = GameResources::GetInstance()->GetTexture("enemyHPBack");
	m_frameTexture = GameResources::GetInstance()->GetTexture("enemyHPFrame");

	// エネミーのHPを取得
	m_enemyHP = m_enemyHPclass->GetMaxHP();
}


// ----------------------------
/// <summary>
/// 更新関数
/// </summary>
// ----------------------------
void EnemyHPUI::Update()
{
}

// ----------------------------
/// <summary>
/// 描画関数
/// </summary>
// ----------------------------
void EnemyHPUI::Render()
{
	// 敵のHPを取得
	m_enemyHP = m_enemyHPclass->GetHP();
	// 最大HPを取得
	float MAXHP = m_enemyHPclass->GetMaxHP();

	// 描画位置のオフセット値や緑ゲージの幅を計算する
	LONG offset = static_cast<LONG>(Screen::CENTER_X - (MAX_WIDTH / 2));
	LONG width = static_cast<LONG>(offset + MAX_WIDTH * (m_enemyHP / MAXHP));

	// ゲージの範囲の設定
	RECT outline{ offset, TOP_POSITION, offset + MAX_WIDTH, BOTTOM_POSITION };
	RECT back	{ offset, TOP_POSITION, offset + MAX_WIDTH, BOTTOM_POSITION };
	RECT gauge	{ offset, TOP_POSITION, width, BOTTOM_POSITION };

	// スプライトバッチを開始する
	m_spriteBatch->Begin();

	// 背面の描画
	m_spriteBatch->Draw(m_backTexture.Get(), back, DirectX::Colors::White);
	// ゲージ部分
	m_spriteBatch->Draw(m_texture.Get(), gauge, DirectX::Colors::White);
	// ゲージの枠
	m_spriteBatch->Draw(m_frameTexture.Get(), outline, DirectX::Colors::White);

	// スプライトバッチを終了する
	m_spriteBatch->End();
}

// ----------------------------
/// <summary>a
/// 終了関数
/// </summary>
// ----------------------------
void EnemyHPUI::Finalize()
{
	m_spriteBatch.reset();
	m_texture.Reset();
	m_enemyHPclass = nullptr;
}