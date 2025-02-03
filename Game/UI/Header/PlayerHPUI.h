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
    static constexpr int MAX_WIDTH = 300;

    // 上から参照
    static constexpr int TOP_POSITION = 50;
    static constexpr int BOTTOM_POSITION = 80;

    static constexpr int LEFT_POSITION = 50;

    static constexpr int FRALE_WIDTH = 4;

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
    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;          // スプライトバッチ
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;    // テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backTexture;    // テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_frameTexture;    // テクスチャ
    DX::DeviceResources* m_pDR;                                    // デバイスリソース

    HPSystem* m_playerHPclass;  // プレイヤーのHPクラス
    float m_playerHP;           // プレイヤーのHP
};

#endif
