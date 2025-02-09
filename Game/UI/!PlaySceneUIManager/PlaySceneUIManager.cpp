// ----------------------------------------------
//
//  名前:  PlaySceneUIManager.h
//  内容:  プレイシーンのUIを管理するクラス
//  作成:  池田桜輔
//
// ----------------------------------------------
// インクルード
#include "pch.h"
#include "Game/CommonResources.h"
#include "PlaySceneUIManager.h"
#include "Game/Screen.h"
#include "Game/Data/GameData.h"
#include "Game/Scene/PlayScene.h"
#include "Game/Player/Player.h"
#include "Game/Boss/Boss.h"
#include "../PlayerHP/PlayerHPUI.h"
#include "../EnemyHP/EnemyHPUI.h"
#include "../Warning/Warning.h"
#include "../Operation/Operation.h"

// -----------------------------------------------------
// 固定値
// -----------------------------------------------------

// プレイヤーのHPの位置
const DirectX::SimpleMath::Vector2 PlaySceneUIManager::PLAYER_HP_POSITION = DirectX::SimpleMath::Vector2(Screen::LEFT + 50.0f, 50.0f);

// -----------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="playScene">プレイシーン</param>
// -----------------------------------------------------
PlaySceneUIManager::PlaySceneUIManager(PlayScene* playScene)
	: 
	m_playScene(playScene)
{
}

// -----------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// -----------------------------------------------------
PlaySceneUIManager::~PlaySceneUIManager()
{
	Finalize();
}

// -----------------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// -----------------------------------------------------
void PlaySceneUIManager::Initialize()
{
	// UIの生成
	CreateUI();
}

// -----------------------------------------------------
/// <summary>
/// UIの生成処理
/// </summary>
// -----------------------------------------------------
void PlaySceneUIManager::CreateUI()
{
	// プレイヤーHPの生成と初期化
	CreatePlayerHPUI();
	CreateEnemyHPUI();
	CreateWarning();
	CreateOperation();
}

// -----------------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// -----------------------------------------------------
void PlaySceneUIManager::Update(float elapsedTime)
{
	// プレイヤーHPの更新
	m_playerHP->Update();

	// エネミーが存在する場合
	if (m_enemyHP)
	{
		// エネミーHPの更新
		m_enemyHP->Update();
	}

	// 警告UIの更新
	m_warning->Update(elapsedTime);
	// 操作UIの更新
	m_operation->Update(elapsedTime);
}

// -----------------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
// -----------------------------------------------------
void PlaySceneUIManager::Render()
{
	// 警告UIの更新
	m_warning->Render();
	// プレイヤーHPの描画
	m_playerHP->Render();

	if (m_enemyHP)
	{
		// 敵HPの描画
		m_enemyHP->Render();
	}

	// チュートリアルでは描画しない
	if (GameData::GetInstance()->GetSelectStage() <= 0) return;

	// 操作UIの更新
	m_operation->Render();
}

// -----------------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
// -----------------------------------------------------
void PlaySceneUIManager::Finalize()
{
	// プレイヤーHPの終了処理
	m_playerHP->Finalize();

	if (m_enemyHP)
	{
		// エネミーHPの終了処理
		m_enemyHP->Finalize();
	}

	// 警告UIの終了処理
	m_warning->Finalize();
	// 操作UIの終了処理
	m_operation->Finalize();
}

// -----------------------------------------------------
/// <summary>
/// プレイヤーHPの生成処理
/// </summary>
// -----------------------------------------------------
void PlaySceneUIManager::CreatePlayerHPUI()
{
	auto playerHP = m_playScene->GetPlayer()->GetPlayerHP();

	if (playerHP == nullptr) MessageBox(0, L"PlaySceneUIManager : プレイヤーのHPが取得できませんでした。", NULL, MB_OK);

	// プレイヤーHPの生成と初期化
	m_playerHP = std::make_unique<PlayerHPUI>(playerHP);
	m_playerHP->Initialize();
}

// -----------------------------------------------------
/// <summary>
/// 敵HPの生成処理
/// </summary>
// -----------------------------------------------------
void PlaySceneUIManager::CreateEnemyHPUI()
{
	// エネミーのHPを取得
	auto enemyHP = m_playScene->GetBoss() ? m_playScene->GetBoss()->GetBossHP() : nullptr;

	// nullチェック
	if (enemyHP != nullptr)
	{
		// エネミーHPの生成と初期化
		m_enemyHP = std::make_unique<EnemyHPUI>(enemyHP);
		m_enemyHP->Initialize();
	}
}

// -----------------------------------------------------
/// <summary>
/// 警告UIの生成処理
/// </summary>
// -----------------------------------------------------
void PlaySceneUIManager::CreateWarning()
{
	// プレイヤーHPの取得
	auto playerHP = m_playScene->GetPlayer()->GetPlayerHP();

	// nullチェック
	if (playerHP == nullptr) MessageBox(0, L"PlaySceneUIManager : プレイヤーのHPが取得できませんでした。", NULL, MB_OK);

	// 警告UIの生成と初期化
	m_warning = std::make_unique<Warning>(playerHP);
	m_warning->Initialize();
}

// -----------------------------------------------------
/// <summary>
/// 操作説明UIの生成処理
/// </summary>
// -----------------------------------------------------
void PlaySceneUIManager::CreateOperation()
{
	// 操作説明UIの生成と初期化
	m_operation = std::make_unique<Operation>();
	m_operation->Initialize();
}