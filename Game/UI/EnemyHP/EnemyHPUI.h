#pragma once

#ifndef ENEMY_HP_UI
#define ENEMY_HP_UI

#include "pch.h"
#include "DeviceResources.h"

class HPSystem;

class EnemyHPUI
{
private:
	const wchar_t* ENEMY_HP_TEXTURE = L"Resources/Textures/UI/1px.png";	// 1pxのテクスチャのパス（画像として一応ね？）

	static constexpr int MAX_WIDTH = 500;

	// 上から参照
	static constexpr int	TOP_POSITION = 650;
	static constexpr int	BOTTOM_POSITION = 680;
	static constexpr float EASING_TIME = 0.5f;

public:
	// コンストラクタ
	EnemyHPUI(HPSystem* hpSystem);
	// デストラクタ
	~EnemyHPUI();

	// 初期化関数
	void Initialize();
	// 更新処理
	void Update();
	// 描画処理
	void Render();
	// 終了処理
	void Finalize();

private:

	// エネミーのHPクラス
	HPSystem* m_enemyHPclass;
	// エネミーのHP
	float m_enemyHP;

	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// HPのテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// 背景テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backTexture;
	// フレームテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_frameTexture;

	// 経過時間
	float m_totalTime;
};

#endif