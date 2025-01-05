// -----------------------------------------------
//
// �v���C����UI�̕`��␶�����s���֐�
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
// �Œ�l
// ----------------------------

// �v���C���[��HP�̈ʒu
const DirectX::SimpleMath::Vector2 PlaySceneUIManager::PLAYER_HP_POSITION = DirectX::SimpleMath::Vector2(Screen::LEFT + 50.0f, 50.0f);


// ----------------------------
// �R���X�g���N�^
// ----------------------------
PlaySceneUIManager::PlaySceneUIManager(PlayScene* playScene)
	: m_playScene(playScene)
{
	// ���\�[�X�̎擾
	m_commonResources = CommonResources::GetInstance();
	m_pDR = m_commonResources->GetDeviceResources();
}


// ----------------------------
// �f�X�g���N�^
// ----------------------------
PlaySceneUIManager::~PlaySceneUIManager()
{
}


// ----------------------------
// �������֐�
// ----------------------------
void PlaySceneUIManager::Initialize()
{
	// UI�̈ꊇ����
	CreateUI();
}


// ----------------------------
// UI�̐����֐�
// ----------------------------
void PlaySceneUIManager::CreateUI()
{
	// HPSystem���擾
	auto playerHP = m_playScene->GetPlayer()->GetPlayerHP();
	auto enemyHP = m_playScene->GetEnemy()->GetEnemyHP();

	// ��������
	m_playerHP	= std::make_unique<PlayerHPUI>	(playerHP);
	m_enemyHP	= std::make_unique<EnemyHPUI>	(enemyHP);
	m_warning	= std::make_unique<Warning>		(playerHP);
	m_operation = std::make_unique<Operation>	();

	// ����������
	m_playerHP	->	Initialize(m_pDR);
	m_enemyHP	->	Initialize(m_pDR);
	m_warning	->	Initialize();
	m_operation	->	Initialize();
}


// ----------------------------
// �X�V�֐�
// ----------------------------
void PlaySceneUIManager::Update(float elapsedTime)
{
	m_playerHP	->	Update();
	m_enemyHP	->	Update();
	m_warning	->	Update(elapsedTime);
	m_operation	->	Update(elapsedTime);
}


// ----------------------------
// �`��֐�
// ----------------------------
void PlaySceneUIManager::Render()
{
	m_warning	->	Render();
	m_playerHP	->	Render();
	m_enemyHP	->	Render();
	m_operation	->	Render();
}


// ----------------------------
// �I���֐�
// ----------------------------
void PlaySceneUIManager::Finalize()
{
	m_playerHP	->	Finalize();
	m_enemyHP	->	Finalize();
	m_warning	->	Finalize();
	m_operation	->	Finalize();
}
