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



// ====================================================================================
const DirectX::SimpleMath::Vector2 PlaySceneUIManager::PLAYER_HP_POSITION = DirectX::SimpleMath::Vector2(Screen::LEFT + 50.0f, 50.0f);



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
	m_playerHP	= std::make_unique<PlayerHPUI>	(m_playScene->GetPlayer()->GetPlayerHP())	;
	m_enemyHP	= std::make_unique<EnemyHPUI>	(m_playScene->GetEnemy ()->GetEnemyHP ())	;
	m_warning	= std::make_unique<Warning>		(m_playScene->GetPlayer()->GetPlayerHP())	;
	m_operation = std::make_unique<Operation>	()											;

	m_playerHP	->	Initialize(m_pDR)	;
	m_enemyHP	->	Initialize(m_pDR)	;
	m_warning	->	Initialize()		;
	m_operation	->	Initialize()		;
}


// ----------------------------
/// <summary>
/// 更新関数
/// </summary>
// ----------------------------
void PlaySceneUIManager::Update(float elapsedTime)
{
	m_playerHP	->Update();
	m_enemyHP	->Update();
	m_warning	->Update(elapsedTime);
	m_operation	->Update(elapsedTime);
}



// ----------------------------
/// <summary>
/// 描画関数
/// </summary>
// ----------------------------
void PlaySceneUIManager::Render()
{
	m_warning	->Render();
	m_playerHP	->Render();
	m_enemyHP	->Render();
	m_operation	->Render();
}



// ----------------------------
/// <summary>
/// 終了関数
/// </summary>
// ----------------------------
void PlaySceneUIManager::Finalize()
{
	m_playerHP	->Finalize();
	m_enemyHP	->Finalize();
	m_warning	->Finalize();
	m_operation	->Finalize();
}
