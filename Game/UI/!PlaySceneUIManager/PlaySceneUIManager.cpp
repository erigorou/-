// -----------------------------------------------
//
// プレイ中のUIの描画や生成を行う関数
//
// -----------------------------------------------

#include "pch.h"
#include "PlaySceneUIManager.h"
#include "Game/Screen.h"
#include "Game/Data/GameData.h"
#include "Game/Scene/PlayScene.h"
#include "Game/Player/Player.h"
#include "Game/Boss/Boss.h"
#include "../Header/PlayerHPUI.h"
#include "../Header/EnemyHPUI.h"
#include "../Header/Warning.h"
#include "../Header/Operation.h"

// ----------------------------
// 固定値
// ----------------------------

// プレイヤーのHPの位置
const DirectX::SimpleMath::Vector2 PlaySceneUIManager::PLAYER_HP_POSITION = DirectX::SimpleMath::Vector2(Screen::LEFT + 50.0f, 50.0f);

// ----------------------------
// コンストラクタ
// ----------------------------
PlaySceneUIManager::PlaySceneUIManager(PlayScene* playScene)
	: m_playScene(playScene)
{
	// リソースの取得
	m_commonResources = CommonResources::GetInstance();
	m_pDR = m_commonResources->GetDeviceResources();
}

// ----------------------------
// デストラクタ
// ----------------------------
PlaySceneUIManager::~PlaySceneUIManager()
{
}

// ----------------------------
// 初期化関数
// ----------------------------
void PlaySceneUIManager::Initialize()
{
}

// ----------------------------
// UIの生成関数
// ----------------------------
void PlaySceneUIManager::CreateUI()
{
	// プレイヤーHPの生成と初期化
	CreatePlayerHPUI();
	CreateEnemyHPUI();
	CreateWarning();
	CreateOperation();
}

// ----------------------------
// 更新関数
// ----------------------------
void PlaySceneUIManager::Update(float elapsedTime)
{
	m_playerHP->Update();

	if (m_enemyHP)	// エネミーが存在する場合
	{
		m_enemyHP->Update();
	}

	m_warning->Update(elapsedTime);
	m_operation->Update(elapsedTime);
}

// ----------------------------
// 描画関数
// ----------------------------
void PlaySceneUIManager::Render()
{
	m_warning->Render();
	m_playerHP->Render();

	if (m_enemyHP)	// エネミーが存在する場合
	{
		m_enemyHP->Render();
	}

	// チュートリアルでは描画しない
	if (GameData::GetInstance()->GetSelectStage() <= 0) return;

	m_operation->Render();
}

// ----------------------------
// 終了関数
// ----------------------------
void PlaySceneUIManager::Finalize()
{
	m_playerHP->Finalize();

	if (m_enemyHP)	// エネミーが存在する場合
	{
		m_enemyHP->Finalize();
	}

	m_warning->Finalize();
	m_operation->Finalize();
}

// ----------------------------
// プレイヤーのHPUIの生成関数
// ----------------------------
void PlaySceneUIManager::CreatePlayerHPUI()
{
	auto playerHP = m_playScene->GetPlayer()->GetPlayerHP();

	if (playerHP == nullptr) MessageBox(0, L"PlaySceneUIManager : プレイヤーのHPが取得できませんでした。", NULL, MB_OK);

	// プレイヤーHPの生成と初期化
	m_playerHP = std::make_unique<PlayerHPUI>(playerHP);
	m_playerHP->Initialize();
}

// ----------------------------
// エネミーのHPUIの生成関数
// ----------------------------
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

// ----------------------------
// 警告UIの生成関数
// ----------------------------
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

// ----------------------------
// 操作説明UIの生成関数
// ----------------------------
void PlaySceneUIManager::CreateOperation()
{
	// 操作説明UIの生成と初期化
	m_operation = std::make_unique<Operation>();
	m_operation->Initialize();
}