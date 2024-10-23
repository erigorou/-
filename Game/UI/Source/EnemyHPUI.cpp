// --------------
//
// EnemyHPUI.cpp
//
// --------------

#include "pch.h"
#include "Game/UI/Header/EnemyHPUI.h"
#include "Game/Enemy/EnemyHP.h"
#include "Game/Screen.h"


// ----------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemyHp">敵のHPを所持するクラスのポインタ</param>
// ----------------------------
EnemyHPUI::EnemyHPUI(EnemyHP* enemyHp)
	: m_enemyHPclass(enemyHp)
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


void EnemyHPUI::Initialize(DX::DeviceResources* pDR)
{
	m_pDR = pDR;
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(pDR->GetD3DDeviceContext());	// スプライトバッチの設定
	
	LoadTexture(ENEMY_HP_TEXTURE);			// テクスチャの読み込み
	m_enemyHP = m_enemyHPclass->GetMaxHP();	// エネミーのHPを取得
}


/// <summary>
/// テクスチャの読み込み関数
/// </summary>
/// <param name="path">ファイルパス</param>
void EnemyHPUI::LoadTexture(const wchar_t* path)
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, m_texture.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to load texture.", NULL, MB_OK);
	}
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
    m_enemyHP	= m_enemyHPclass->GetHP();		// 敵のHPを取得
    int MAXHP	= m_enemyHPclass->GetMaxHP();	// 最大HPを取得

    // 描画位置のオフセット値や緑ゲージの幅を計算する
    LONG offset = static_cast<LONG>(Screen::CENTER_X - (MAX_WIDTH / 2));
    LONG width	= static_cast<LONG>(offset + MAX_WIDTH * (m_enemyHP / MAXHP));

	// ゲージの範囲の設定
    RECT outline{ offset - 2, TOP_POSITION - 2, offset + MAX_WIDTH + 2, BOTTOM_POSITION + 2 };
    RECT back{ offset, TOP_POSITION, offset + MAX_WIDTH, BOTTOM_POSITION };
    RECT gauge{ offset, TOP_POSITION, width, BOTTOM_POSITION };

    // スプライトバッチを開始する
    m_spriteBatch->Begin();

    // ゲージの描画
    m_spriteBatch->Draw(m_texture.Get(), outline, DirectX::Colors::Black);
    m_spriteBatch->Draw(m_texture.Get(), back, DirectX::Colors::BlanchedAlmond);  // 背面の描画
    m_spriteBatch->Draw(m_texture.Get(), gauge, DirectX::Colors::Red);            // ゲージ部分

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
	m_pDR = nullptr;
	m_enemyHPclass = nullptr;
}