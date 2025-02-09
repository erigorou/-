#pragma once

#ifndef PLAYER_HP_UI
#define PLAYER_HP_UI

#include "pch.h"
#include "DeviceResources.h"

class HPSystem;
class CustomShader;

class PlayerHPUI
{
private:
    // 最大横幅
    static constexpr int MAX_WIDTH = 300;
	// Y座標（上）
    static constexpr int TOP_POSITION = 50;
	// Y座標（下）
    static constexpr int BOTTOM_POSITION = 80;
	// X座標(左)
    static constexpr int LEFT_POSITION = 50;

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
