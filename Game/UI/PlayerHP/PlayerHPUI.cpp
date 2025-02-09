// --------------------------------------------
// 
// 名前:　　PlayerHPUI.cpp
// 機能:	プレイヤーのHPを表示するUIクラス
// 製作:	池田桜輔
// 
// --------------------------------------------

// インクルード
#include "pch.h"
#include "PlayerHPUI.h"
#include "Game/Data/HPSystem.h"
#include "Game/GameResources.h"
#include "Game/CommonResources.h"

// ----------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="hpSystem">プレイヤーHPシステムのポインタ</param>
// ----------------------------
PlayerHPUI::PlayerHPUI(HPSystem* hpSystem)
    : m_playerHPclass(hpSystem),
    m_spriteBatch{},
    m_playerHP{}
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

// ----------------------------
/// <summary>
/// 初期化関数
/// </summary>
// ----------------------------
void PlayerHPUI::Initialize()
{
    // スプライトバッチの設定
    auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
    m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

    // プレイヤーのHPを取得
    m_playerHP = m_playerHPclass->GetMaxHP();

    // テクスチャの読み込み
    m_texture = GameResources::GetInstance()->GetTexture("playerHP");
	m_backTexture = GameResources::GetInstance()->GetTexture("playerHPBack");
	m_frameTexture = GameResources::GetInstance()->GetTexture("playerHPFrame");
}

// ----------------------------
/// <summary>
/// 更新処理
/// </summary>
// ----------------------------
void PlayerHPUI::Update()
{
    // プレイヤーのHPを取得
    m_playerHP = m_playerHPclass->GetHP();
}

// ----------------------------
/// <summary>
/// 描画処理
/// </summary>
// ----------------------------
void PlayerHPUI::Render()
{
    float MAXHP = static_cast<float>(m_playerHPclass->GetMaxHP());  // 最大HPを取得

    // 描画位置のオフセット値や緑ゲージの幅を計算する
    LONG offset = static_cast<LONG>(LEFT_POSITION);
    LONG width = static_cast<LONG>(offset + MAX_WIDTH * (m_playerHP / MAXHP));

    // ゲージの範囲の設定
    RECT outline{ offset, TOP_POSITION, offset + MAX_WIDTH, BOTTOM_POSITION };
    RECT back   { offset, TOP_POSITION, offset + MAX_WIDTH, BOTTOM_POSITION };
    RECT gauge  { offset, TOP_POSITION, width, BOTTOM_POSITION };

	// スプライトバッチを開始する
    m_spriteBatch->Begin();

    // 背面の描画
    m_spriteBatch->Draw(m_backTexture.Get(), back,DirectX::Colors::White);
    // ゲージ部分
    m_spriteBatch->Draw(m_texture.Get(), gauge, DirectX::Colors::White);
    // ゲージの枠
    m_spriteBatch->Draw(m_frameTexture.Get(), outline, DirectX::Colors::White);

    // スプライトバッチを終了する
    m_spriteBatch->End();

}

// ----------------------------
/// <summary>
/// 終了処理
/// </summary>
// ----------------------------
void PlayerHPUI::Finalize()
{
    // リソースの解放
    m_spriteBatch.reset();
    m_texture.Reset();
    m_playerHPclass = nullptr;
}
