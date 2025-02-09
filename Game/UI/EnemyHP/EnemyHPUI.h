// --------------------------------------------
//
// 名前:	EnemyHPUI.cpp
// 機能:	ボスのHPを表示するUIクラス
// 製作:	池田桜輔
//
// --------------------------------------------
#ifndef ENEMY_HP_UI
#define ENEMY_HP_UI
#pragma once
// インクルード
#include "pch.h"
#include "DeviceResources.h"

// 前方宣言
class HPSystem;

/// <summary>
/// ボスHPUIクラス
/// </summary>
class EnemyHPUI
{
	// ------------------------------
	// 固定値
	// ------------------------------	
private:
	// テクスチャのサイズ
	static constexpr int MAX_WIDTH = 500;
	// テクスチャの位置
	static constexpr int TOP_POSITION = 650;
	// テクスチャの位置
	static constexpr int BOTTOM_POSITION = 680;

	// ------------------------------
	// メンバ関数
	// ------------------------------
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

	// ------------------------------
	// メンバ変数
	// ------------------------------
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