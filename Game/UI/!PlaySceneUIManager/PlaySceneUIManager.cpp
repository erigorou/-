// -----------------------------------------------
//
// プレイ中のUIの描画や生成を行う関数
//
// -----------------------------------------------

#include "pch.h"
#include "PlaySceneUIManager.h"
#include "Game/Screen.h"
#include "Game/Scene/PlayScene.h"
#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
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
	// UIの一括生成
	CreateUI();
}


// ----------------------------
// UIの生成関数
// ----------------------------
void PlaySceneUIManager::CreateUI()
{
	// HPSystemを取得
	auto playerHP = m_playScene->GetPlayer()->GetPlayerHP();
	auto enemyHP = m_playScene->GetEnemy()->GetEnemyHP();

	// 生成処理
	m_playerHP	= std::make_unique<PlayerHPUI>	(playerHP);
	m_enemyHP	= std::make_unique<EnemyHPUI>	(enemyHP);
	m_warning	= std::make_unique<Warning>		(playerHP);
	m_operation = std::make_unique<Operation>	();

	// 初期化処理
	m_playerHP	->	Initialize(m_pDR);
	m_enemyHP	->	Initialize(m_pDR);
	m_warning	->	Initialize();
	m_operation	->	Initialize();
}


// ----------------------------
// 更新関数
// ----------------------------
void PlaySceneUIManager::Update(float elapsedTime)
{
	m_playerHP	->	Update();
	m_enemyHP	->	Update();
	m_warning	->	Update(elapsedTime);
	m_operation	->	Update(elapsedTime);
}


// ----------------------------
// 描画関数
// ----------------------------
void PlaySceneUIManager::Render()
{
	m_warning	->	Render();
	m_playerHP	->	Render();
	m_enemyHP	->	Render();
	m_operation	->	Render();
}


// ----------------------------
// 終了関数
// ----------------------------
void PlaySceneUIManager::Finalize()
{
	m_playerHP	->	Finalize();
	m_enemyHP	->	Finalize();
	m_warning	->	Finalize();
	m_operation	->	Finalize();
}
