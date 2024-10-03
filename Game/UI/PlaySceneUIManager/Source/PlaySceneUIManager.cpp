// -----------------------------------------------
//
// プレイ中のUIの描画や生成を行う関数
//
// -----------------------------------------------

#include "pch.h"
#include "Game/UI/PlaySceneUIManager/Header/PlaySceneUIManager.h"
#include "Game/Screen.h"
#include "Game/Scene/PlayScene.h"
#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/UI/Header/PlayerHPUI.h"
#include "Game/UI/Header/EnemyHPUI.h"



// ====================================================================================
const DirectX::SimpleMath::Vector2 PlaySceneUIManager::PLAYER_HP_POSITION = DirectX::SimpleMath::Vector2(Screen::CENTER_X - 100, 650.0f);



// ----------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// ----------------------------
PlaySceneUIManager::PlaySceneUIManager(PlayScene* playScene)
	: m_playScene(playScene)
{
	m_commonResources = CommonResources::GetInstance();
	m_pDR = m_commonResources->GetDeviceResources();
}



// ----------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ----------------------------
PlaySceneUIManager::~PlaySceneUIManager()
{
}



// ----------------------------
/// <summary>
/// 初期化関数
/// </summary>
// ----------------------------
void PlaySceneUIManager::Initialize()
{
	CreateUI();
}


// ----------------------------
/// <summary>
/// UIの生成関数
/// </summary>
// ----------------------------
void PlaySceneUIManager::CreateUI()
{
	m_playerHP = std::make_unique<PlayerHPUI>	(m_playScene->GetPlayer()->GetPlayerHP());
	m_enemyHP  = std::make_unique<EnemyHPUI>	(m_playScene->GetEnemy ()->GetEnemyHP ());

	m_playerHP-> Initialize(m_pDR, PLAYER_HP_POSITION, DirectX::SimpleMath::Vector2::One);
	m_enemyHP -> Initialize(m_pDR);
}


// ----------------------------
/// <summary>
/// 更新関数
/// </summary>
// ----------------------------
void PlaySceneUIManager::Update()
{
	m_playerHP->Update();
	m_enemyHP ->Update();
}



// ----------------------------
/// <summary>
/// 描画関数
/// </summary>
// ----------------------------
void PlaySceneUIManager::Render()
{
	m_playerHP->Render();
	m_enemyHP ->Render();
}



// ----------------------------
/// <summary>
/// 終了関数
/// </summary>
// ----------------------------
void PlaySceneUIManager::Finalize()
{
	m_playerHP->Finalize();
	m_enemyHP->Finalize();
}
