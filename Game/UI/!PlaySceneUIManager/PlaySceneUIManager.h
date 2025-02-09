// ----------------------------------------------
//
//  名前:  PlaySceneUIManager.h
//  内容:  プレイシーンのUIを管理するクラス
//  作成:  池田桜輔
//
// ----------------------------------------------
#ifndef PLAYSCENE_UI_MANAGER
#define PLAYSCENE_UI_MANAGER
#pragma once
// インクルード
#include "pch.h"

// 前方宣言
class IUserInterface;
class PlayScene;
class Player;
class PlayerHPUI;
class EnemyHPUI;
class Warning;
class Operation;

/// <summary>
/// プレイシーンUIのマネージャー
/// </summary>
class PlaySceneUIManager
{
	// -----------------------------
	// 固定値
	// -----------------------------
public:
	// プレイヤーのHPUIの座標
	static const DirectX::SimpleMath::Vector2 PLAYER_HP_POSITION;

	// -----------------------------
	// メンバ関数(公開)
	// -----------------------------
public:
	// コンストラクタ
	PlaySceneUIManager(PlayScene* playScene);
	// デストラクタ
	~PlaySceneUIManager();
	// 初期化関数
	void Initialize();
	// 更新関数
	void Update(float elapsedTime);
	// 描画関数
	void Render();
	// 終了関数
	void Finalize();

	// -----------------------------
	// メンバ関数(非公開)
	// -----------------------------
private:
	// UIの生成関数
	void CreateUI();
	// プレイヤーのHPUIの生成関数
	void CreatePlayerHPUI();
	// エネミーのHPUIの生成関数
	void CreateEnemyHPUI();
	// 警告UIの生成関数
	void CreateWarning();
	// 操作説明UIの生成関数
	void CreateOperation();

	// -----------------------------
	// メンバ変数
	// -----------------------------
private:
	// プレイシーン
	PlayScene* m_playScene;
	// プレイヤーのHP
	std::unique_ptr<PlayerHPUI> m_playerHP;
	// エネミーのHP
	std::unique_ptr<EnemyHPUI> m_enemyHP;
	// 警告UI
	std::unique_ptr<Warning> m_warning;
	// 操作説明UI
	std::unique_ptr<Operation> m_operation;
};

#endif
