// --------------------------------------------
// 
// 名前:　　PlayerHPUI.cpp
// 機能:	プレイヤーのHPを表示するUIクラス
// 製作:	池田桜輔
// 
// --------------------------------------------
#ifndef PLAYER_HP_UI
#define PLAYER_HP_UI

#pragma once
// インクルード
#include "pch.h"
#include "DeviceResources.h"

// 前方宣言
class HPSystem;
class CustomShader;

/// <summary>
/// プレイヤーHPUIクラス
/// </summary>
class PlayerHPUI
{
    // ------------------------------
	// 固定値
	// ------------------------------
private:
    // 最大横幅
    static constexpr int MAX_WIDTH = 300;
	// Y座標（上）
    static constexpr int TOP_POSITION = 50;
	// Y座標（下）
    static constexpr int BOTTOM_POSITION = 80;
	// X座標(左)
    static constexpr int LEFT_POSITION = 50;

    // ------------------------------
    // メンバ関数
    // ------------------------------
public:
    // コンストラクタ
    PlayerHPUI(HPSystem* HPSystem);
    // デストラクタ
    ~PlayerHPUI();
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
    // スプライトバッチ
    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
    // HPのテクスチャ
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
    // 背景テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backTexture;
    // フレームテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_frameTexture;
    // プレイヤーのHPクラス
    HPSystem* m_playerHPclass;
    // プレイヤーのHP
    float m_playerHP;
};

#endif
