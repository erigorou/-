// -----------------------------------------------
//
// �v���C����UI�̕`��␶�����s���֐�
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
	// �v���C���[��HP
	m_playerHP = std::make_unique<PlayerHPUI>(m_playScene->GetPlayer()->GetPlayerHP());
	m_playerHP->Initialize(m_pDR, DirectX::SimpleMath::Vector2(0, 0), DirectX::SimpleMath::Vector2(0.5, 0.5));
}


// ----------------------------
/// <summary>
/// �X�V�֐�
/// </summary>
// ----------------------------
void PlaySceneUIManager::Update()
{
	m_playerHP->Update();
}



// ----------------------------
/// <summary>
/// �`��֐�
/// </summary>
// ----------------------------
void PlaySceneUIManager::Render()
{
	m_playerHP->Render();
}



// ----------------------------
/// <summary>
/// �I���֐�
/// </summary>
// ----------------------------
void PlaySceneUIManager::Finalize()
{

}
