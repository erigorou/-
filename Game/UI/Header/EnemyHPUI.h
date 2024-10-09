#pragma once

#ifndef ENEMY_HP_UI
#define ENEMY_HP_UI

#include "pch.h"
#include "Libraries/Microsoft/DebugDraw.h"
#include "DeviceResources.h"

class EnemyHP;

class EnemyHPUI
{
private:
	const wchar_t* ENEMY_HP_TEXTURE = L"Resources/Textures/UI/1px.png";	// 1pxのテクスチャのパス（画像として一応ね？）

	static constexpr int MAX_WIDTH			= 500;

	static constexpr int TOP_POSITION		= 25;
	static constexpr int BOTTOM_POSITION	= 50;

public:
	EnemyHPUI(EnemyHP* enemyHp);	// コンストラクタ
	~EnemyHPUI();					// デストラクタ

	void Initialize(DX::DeviceResources* pDR);	// 初期化関数
	void LoadTexture(const wchar_t* path);		// テクスチャの読み込み

	void Update();		// 更新処理
	void Render();		// 描画処理
	void Finalize();	// 終了処理

private:

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;		// スプライトバッチ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;	// テクスチャ
	DX::DeviceResources* m_pDR;									// デバイスリソース


	EnemyHP* m_enemyHPclass;	// エネミーのHPクラス
	int m_enemyHP;				// エネミーのHP


};

#endif