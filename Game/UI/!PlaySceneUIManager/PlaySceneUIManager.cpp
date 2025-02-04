// -----------------------------------------------
//
// �v���C����UI�̕`��␶�����s���֐�
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
}

// ----------------------------
// UI�̐����֐�
// ----------------------------
void PlaySceneUIManager::CreateUI()
{
	// �v���C���[HP�̐����Ə�����
	CreatePlayerHPUI();
	CreateEnemyHPUI();
	CreateWarning();
	CreateOperation();
}

// ----------------------------
// �X�V�֐�
// ----------------------------
void PlaySceneUIManager::Update(float elapsedTime)
{
	m_playerHP->Update();

	if (m_enemyHP)	// �G�l�~�[�����݂���ꍇ
	{
		m_enemyHP->Update();
	}

	m_warning->Update(elapsedTime);
	m_operation->Update(elapsedTime);
}

// ----------------------------
// �`��֐�
// ----------------------------
void PlaySceneUIManager::Render()
{
	m_warning->Render();
	m_playerHP->Render();

	if (m_enemyHP)	// �G�l�~�[�����݂���ꍇ
	{
		m_enemyHP->Render();
	}

	// �`���[�g���A���ł͕`�悵�Ȃ�
	if (GameData::GetInstance()->GetSelectStage() <= 0) return;

	m_operation->Render();
}

// ----------------------------
// �I���֐�
// ----------------------------
void PlaySceneUIManager::Finalize()
{
	m_playerHP->Finalize();

	if (m_enemyHP)	// �G�l�~�[�����݂���ꍇ
	{
		m_enemyHP->Finalize();
	}

	m_warning->Finalize();
	m_operation->Finalize();
}

// ----------------------------
// �v���C���[��HPUI�̐����֐�
// ----------------------------
void PlaySceneUIManager::CreatePlayerHPUI()
{
	auto playerHP = m_playScene->GetPlayer()->GetPlayerHP();

	if (playerHP == nullptr) MessageBox(0, L"PlaySceneUIManager : �v���C���[��HP���擾�ł��܂���ł����B", NULL, MB_OK);

	// �v���C���[HP�̐����Ə�����
	m_playerHP = std::make_unique<PlayerHPUI>(playerHP);
	m_playerHP->Initialize();
}

// ----------------------------
// �G�l�~�[��HPUI�̐����֐�
// ----------------------------
void PlaySceneUIManager::CreateEnemyHPUI()
{
	// �G�l�~�[��HP���擾
	auto enemyHP = m_playScene->GetBoss() ? m_playScene->GetBoss()->GetBossHP() : nullptr;

	// null�`�F�b�N
	if (enemyHP != nullptr)
	{
		// �G�l�~�[HP�̐����Ə�����
		m_enemyHP = std::make_unique<EnemyHPUI>(enemyHP);
		m_enemyHP->Initialize();
	}
}

// ----------------------------
// �x��UI�̐����֐�
// ----------------------------
void PlaySceneUIManager::CreateWarning()
{
	// �v���C���[HP�̎擾
	auto playerHP = m_playScene->GetPlayer()->GetPlayerHP();

	// null�`�F�b�N
	if (playerHP == nullptr) MessageBox(0, L"PlaySceneUIManager : �v���C���[��HP���擾�ł��܂���ł����B", NULL, MB_OK);

	// �x��UI�̐����Ə�����
	m_warning = std::make_unique<Warning>(playerHP);
	m_warning->Initialize();
}

// ----------------------------
// �������UI�̐����֐�
// ----------------------------
void PlaySceneUIManager::CreateOperation()
{
	// �������UI�̐����Ə�����
	m_operation = std::make_unique<Operation>();
	m_operation->Initialize();
}