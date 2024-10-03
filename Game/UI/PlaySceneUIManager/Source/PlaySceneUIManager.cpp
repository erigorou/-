// -----------------------------------------------
//
// プレイ中のUIの描画や生成を行う関数
//
// -----------------------------------------------

#include "pch.h"
#include "Game/UI/PlaySceneUIManager/Header/PlaySceneUIManager.h"
#include "Game/Scene/PlayScene.h"
#include "Game/Player/Player.h"

#include "Interface/IUserInterface.h"
#include "Game/UI/Header/PlayerHPUI.h"




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
	// プレイヤーのHP
	m_playerHP = std::make_unique<PlayerHPUI>(m_playScene->GetPlayer()->GetPlayerHP());
	m_playerHP->Initialize(m_pDR, DirectX::SimpleMath::Vector2(0, 0), DirectX::SimpleMath::Vector2(0.5, 0.5));
}


// ----------------------------
/// <summary>
/// 更新関数
/// </summary>
// ----------------------------
void PlaySceneUIManager::Update()
{
	m_playerHP->Update();
}



// ----------------------------
/// <summary>
/// 描画関数
/// </summary>
// ----------------------------
void PlaySceneUIManager::Render()
{
	m_playerHP->Render();
}



// ----------------------------
/// <summary>
/// 終了関数
/// </summary>
// ----------------------------
void PlaySceneUIManager::Finalize()
{

}
