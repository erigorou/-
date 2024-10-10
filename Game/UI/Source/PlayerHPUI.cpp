// --------------
//
// PlayerHPUI.cpp
//
// --------------

#include "pch.h"
#include "Game/UI/Header/PlayerHPUI.h"
#include "Game/Player/PlayerHP.h"

// ----------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// ----------------------------
PlayerHPUI::PlayerHPUI(PlayerHP* playerHp)
	: m_playerHPclass(playerHp)
	, m_spriteBatch()
	, m_texture()
	, m_pDR(nullptr)
	, m_position()
	, m_scale()
	, m_isDead(false)
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
/// <param name="position">座標</param>
/// <param name="size">大きさ</param>
// ----------------------------
void PlayerHPUI::Initialize(DX::DeviceResources* pDR, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale)
{
	m_pDR = pDR;
	m_position = position;
	m_scale = scale;
	m_playerHP = m_playerHPclass->GetMaxHP();	// プレイヤーのHPを取得

	m_windowWidth = m_pDR->GetScreenViewport().Width;
	m_windowHeight = m_pDR->GetScreenViewport().Height;

	// 開始時に５つアイコンを生成する
	for (int i = 0; i < m_playerHP; i++)
	{
		Add(HPUI_FILEPATH, position, scale, ANCHOR::MIDDLE_CENTER);

		m_HPIcons[i]->SetPosition(DirectX::SimpleMath::Vector2(position.x + (50.0f * i), position.y));
	}
}

// ----------------------------
/// <summary>
/// 描画処理
/// </summary>
// ----------------------------
void PlayerHPUI::Update()
{
}


// ----------------------------
/// <summary>
/// 描画処理
/// </summary>
// ----------------------------
void PlayerHPUI::Render()
{
	m_playerHP = m_playerHPclass->GetHP();	// プレイヤーのHPを取得

	// アイコンの描画
	for (int i = 0; i < m_playerHP; i++)
	{
		m_HPIcons[i]->Render();
	}
}


// ----------------------------
/// <summary>
/// HPUIのアイコンを追加する関数
/// </summary>
/// <param name="path">ファイルパス</param>
/// <param name="position">座標</param>
/// <param name="scale">大きさ</param>
/// <param name="anchor">アンカー</param>
// ----------------------------
void PlayerHPUI::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, ANCHOR anchor)
{
	std::unique_ptr<IconUI> icon = std::make_unique<IconUI>();
	icon->Create(m_pDR, path, position, scale, anchor);
	icon->SetWindowSize(m_windowWidth, m_windowHeight);
	m_HPIcons.push_back(std::move(icon));
}



// ----------------------------
/// <summary>
/// 終了処理
/// </summary>
// ----------------------------
void PlayerHPUI::Finalize()
{
}


