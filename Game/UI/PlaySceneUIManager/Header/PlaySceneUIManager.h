#pragma once

#ifndef PLAYSCENE_UI_MANAGER
#define PLAYSCENE_UI_MANAGER

#include "pch.h"
#include "Libraries/Microsoft/DebugDraw.h"

// 前方宣言
class IUserInterface;
class PlayScene;
class Player;
class PlayerHPUI;
class CommonResources; // CommonResources の前方宣言
namespace DX {
    class DeviceResources; // DX::DeviceResources の前方宣言
}

class PlaySceneUIManager
{
public:

    // コンストラクタ
    PlaySceneUIManager(PlayScene* playScene);
    // デストラクタ
    ~PlaySceneUIManager();
    // 初期化関数
    void Initialize();
    // UI生成関数
    void CreateUI();
    // 更新関数
    void Update();
    // 描画関数
    void Render();
    // 終了関数
    void Finalize();

private:
    PlayScene* m_playScene;         // プレイシーン
    DX::DeviceResources* m_pDR;     // デバイスリソース
    CommonResources* m_commonResources; // 共通リソース

    std::unique_ptr<PlayerHPUI> m_playerHP; // プレイヤーのHP
};

#endif
