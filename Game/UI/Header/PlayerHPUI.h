#pragma once

#ifndef PLAYER_HP_UI
#define PLAYER_HP_UI

// プリコンパイルヘッダー
#include "pch.h"
#include "Game/UI/Header/IconUI.h"

class PlayerHP;

// プレイヤーのHPを描画するクラス
class PlayerHPUI
{
public:
	PlayerHPUI(PlayerHP* playerHp);	// コンストラクタ
	~PlayerHPUI();					// デストラクタ
	
	void Initialize(DX::DeviceResources* pDR, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale);	// 初期化関数
	void Update();		// 更新処理
	void Render();		// 描画処理
	void Finalize();	// 終了処理

	void Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, ANCHOR anchor);	// アイコンを追加する関数
	void Delete(std::vector<std::unique_ptr<IconUI>> ui);

private:
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;		// スプライトバッチ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;	// テクスチャ
	DX::DeviceResources* m_pDR;

	DirectX::SimpleMath::Vector2 m_position;	// 座標
	DirectX::SimpleMath::Vector2 m_scale;		// アイコン一つの大きさ

	int m_windowWidth, m_windowHeight;	// ウィンドウの大きさ

	int m_playerHP;	// プレイヤーのHP
	bool m_isDead;	// プレイヤーが死んでいるかどうか

	PlayerHP* m_playerHPclass;	// プレイヤーのHPクラス

	std::vector<std::unique_ptr<IconUI>> m_HPIcons;	// HPのアイコン（複数）




	const wchar_t* HPUI_FILEPATH = L"Resources/Textures/UI/HPICON.png";	// HPのテクスチャのパス
};

#endif