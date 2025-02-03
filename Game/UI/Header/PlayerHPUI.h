#pragma once

#ifndef PLAYER_HP_UI
#define PLAYER_HP_UI

#include "pch.h"
#include "DeviceResources.h"

class HPSystem;

class PlayerHPUI
{
private:
	const wchar_t* ENEMY_HP_TEXTURE = L"Resources/Textures/UI/1px.png";	// 1pxのテクスチャのパス（画像として一応ね？）

	static constexpr int MAX_WIDTH = 300;

	// 上から参照
	static constexpr int TOP_POSITION = 50;
	static constexpr int BOTTOM_POSITION = 80;

	static constexpr int LEFT_POSITION = 50;

public:
	PlayerHPUI(HPSystem* HPSystem);	// コンストラクタ
	~PlayerHPUI();					// デストラクタ

	void Initialize(DX::DeviceResources* pDR);	// 初期化関数
	void LoadTexture(const wchar_t* path);		// テクスチャの読み込み

	void Update();		// 更新処理
	void Render();		// 描画処理
	void Finalize();	// 終了処理

private:

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;		// スプライトバッチ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;	// テクスチャ
	DX::DeviceResources* m_pDR;									// デバイスリソース

	HPSystem* m_playerHPclass;	// プレイヤーのHPクラス
	float m_playerHP;				// プレイヤーのHP
};

#endif