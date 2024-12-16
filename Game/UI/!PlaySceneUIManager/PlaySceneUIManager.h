#pragma once

#ifndef PLAYSCENE_UI_MANAGER
#define PLAYSCENE_UI_MANAGER

#include "pch.h"

// 前方宣言
class IUserInterface;
class PlayScene;
class Player;
class PlayerHPUI;
class EnemyHPUI;
class Warning;
class Operation;

class CommonResources; // CommonResources の前方宣言

namespace DX {
    class DeviceResources; // DX::DeviceResources の前方宣言
}

class PlaySceneUIManager
{
public:

	// ====================================================================================
	static const DirectX::SimpleMath::Vector2 PLAYER_HP_POSITION; // プレイヤーのHPUIの座標


	// ====================================================================================

    PlaySceneUIManager(PlayScene* playScene);   // コンストラクタ
	~PlaySceneUIManager();                      // デストラクタ

	void Initialize	();  					// 初期化関数
	void CreateUI	();						// UIの生成関数
	void Update		(float elapsedTime);	// 更新関数
	void Render		();						// 描画関数
	void Finalize	();						// 終了関数

private:
    PlayScene* m_playScene;				// プレイシーン
    DX::DeviceResources* m_pDR;			// デバイスリソース
    CommonResources* m_commonResources;	// 共通リソース

    std::unique_ptr<PlayerHPUI>	m_playerHP;		// プレイヤーのHP
	std::unique_ptr<EnemyHPUI>	m_enemyHP;		// エネミーのHP
	std::unique_ptr<Warning>	m_warning;		// 警告UI
	std::unique_ptr<Operation>	m_operation;	// 操作説明UI
};

#endif
