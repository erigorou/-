// -----------------------------------------------
//
// �v���C����UI�̕`��␶�����s���֐�
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
/// �R���X�g���N�^
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
/// �f�X�g���N�^
/// </summary>
// ----------------------------
PlaySceneUIManager::~PlaySceneUIManager()
{
}



// ----------------------------
/// <summary>
/// �������֐�
/// </summary>
// ----------------------------
void PlaySceneUIManager::Initialize()
{
	CreateUI();
}


// ----------------------------
/// <summary>
/// UI�̐����֐�
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
/// �X�V�֐�
/// </summary>
// ----------------------------
void PlaySceneUIManager::Update()
{
	m_playerHP->Update();
	m_enemyHP ->Update();
}



// ----------------------------
/// <summary>
/// �`��֐�
/// </summary>
// ----------------------------
void PlaySceneUIManager::Render()
{
	m_playerHP->Render();
	m_enemyHP ->Render();
}



// ----------------------------
/// <summary>
/// �I���֐�
/// </summary>
// ----------------------------
void PlaySceneUIManager::Finalize()
{
	m_playerHP->Finalize();
	m_enemyHP->Finalize();
}
