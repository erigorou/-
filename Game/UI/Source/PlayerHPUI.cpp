// --------------
//
// PlayerHPUI.cpp
//
// --------------

#include "pch.h"
#include "Game/UI/Header/PlayerHPUI.h"
#include "Game/Player/PlayerHP.h"
#include "Game/Screen.h"


// ----------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemyHp">敵のHPを所持するクラスのポインタ</param>
// ----------------------------
PlayerHPUI::PlayerHPUI(PlayerHP* playerHp)
	: m_playerHPclass(playerHp)
	, m_spriteBatch()
	, m_playerHP(0)
{
}

// ----------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ----------------------------
PlayerHPUI::~PlayerHPUI()
{
}


void PlayerHPUI::Initialize(DX::DeviceResources* pDR)
{
	m_pDR = pDR;
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(pDR->GetD3DDeviceContext());	// スプライトバッチの設定
	
	LoadTexture(ENEMY_HP_TEXTURE);				// テクスチャの読み込み
	m_playerHP = m_playerHPclass->GetMaxHP();	// プレイヤーのHPを取得
}


/// <summary>
/// テクスチャの読み込み関数
/// </summary>
/// <param name="path">ファイルパス</param>
void PlayerHPUI::LoadTexture(const wchar_t* path)
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
void PlayerHPUI::Update()
{
}


// ----------------------------
/// <summary>
/// 描画関数
/// </summary>
// ----------------------------
// ----------------------------
/// <summary>
/// 描画関数
/// </summary>
// ----------------------------
void PlayerHPUI::Render()
{
    m_playerHP	= m_playerHPclass->GetHP();		// 敵のHPを取得
    int MAXHP	= m_playerHPclass->GetMaxHP();	// 最大HPを取得

    // 描画位置のオフセット値や緑ゲージの幅を計算する
    LONG offset = static_cast<LONG>(LEFT_POSITION);
    LONG width = static_cast<LONG>(offset + MAX_WIDTH * (m_playerHP / MAXHP));

	// ゲージの範囲の設定
    RECT outline{ offset - 2, TOP_POSITION - 2, offset + MAX_WIDTH + 2, BOTTOM_POSITION + 2 };
    RECT back{ offset, TOP_POSITION, offset + MAX_WIDTH, BOTTOM_POSITION };
    RECT gauge{ offset, TOP_POSITION, width, BOTTOM_POSITION };

    // スプライトバッチを開始する
    m_spriteBatch->Begin();

    // ゲージの描画
    m_spriteBatch->Draw(m_texture.Get(), outline, DirectX::Colors::Black);
    m_spriteBatch->Draw(m_texture.Get(), back, DirectX::Colors::BlanchedAlmond);  // 背面の描画
    m_spriteBatch->Draw(m_texture.Get(), gauge, DirectX::Colors::LimeGreen);            // ゲージ部分

    // スプライトバッチを終了する
    m_spriteBatch->End();
}


// ----------------------------
/// <summary>a
/// 終了関数
/// </summary>
// ----------------------------
void PlayerHPUI::Finalize()
{
	m_spriteBatch.reset();
	m_texture.Reset();
	m_pDR = nullptr;
	m_playerHPclass = nullptr;
}