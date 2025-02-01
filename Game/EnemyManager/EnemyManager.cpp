// ---------------------------------------------
// �u���e�v�S�Ă̓G�̊Ǘ����s���N���X
// �u���t�v12/18
// �u�쐬�ҁv�r�c
// ---------------------------------------------

#pragma once
#include "pch.h"
#include"Game/Scene/PlayScene.h"
#include "Game/Data/GameData.h"
#include "Game/UI/!PlaySceneUIManager/PlaySceneUIManager.h"
#include "EnemyManager.h"
#include "Game/Messenger/EventMessenger.h"
#include "../Boss/Boss.h"
#include "../Goblin/Goblin.h"
#include "../Factory/Factory.h"
#include "../Scene/PlayScene.h"
#include "Interface/IEnemy.h"
#include "Game/CommonResources.h"

#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"

// --------------------------------
// �R���X�g���N�^
// --------------------------------
EnemyManager::EnemyManager(PlayScene* playScene)
	: m_targetEnemyIndex()
	, m_playScene(playScene)
	, m_elapsedTime()
	, m_currentTime()
{
	// �Z���N�g�N�G�X�g�̃C���f�b�N�X���擾
	m_selectQuestIndex = GameData::GetInstance()->GetSelectStage();

	// Json�t�@�C������G�𐶐�
	GenerateEnemyFromJson();
}



// --------------------------------
// �f�X�g���N�^
// --------------------------------
EnemyManager::~EnemyManager()
{
}


// --------------------------------
// ����������
// --------------------------------
void EnemyManager::Initialize(PlayScene* playScene)
{
	m_playScene = playScene;

	// �C�x���g���b�Z���W���[�ɓo�^
	EventMessenger::Attach("EnemyCanHit", std::bind(&EnemyManager::AllEnemyCanHit, this, std::placeholders::_1));
	EventMessenger::Attach("DeleteAllGoblin", std::bind(&EnemyManager::DeleteAllGoblin, this));
}


// --------------------------------
// �X�V����
// --------------------------------
void EnemyManager::Update(float elapsedTime)
{
	m_elapsedTime = elapsedTime;

	if (m_enemies.empty()) return;

	for (auto& enemy : m_enemies)
	{
		enemy.data->Update(elapsedTime);
	}
}


// --------------------------------
// �`�揈��
// --------------------------------
void EnemyManager::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	if (m_enemies.empty()) return;

	// �S�u�����A�{�X�̕`��
	for (auto& enemy : m_enemies)
	{
		enemy.data->Render(view, projection);
	}
}


// --------------------------------
// �I������
// --------------------------------
void EnemyManager::Finalize()
{
	if (m_enemies.empty()) return;

	// �S�u�����A�{�X�̉��
	for (auto& enemy : m_enemies)
	{
		enemy.data->Finalize();
	}
	m_enemies.clear();
}


// --------------------------------
// �G�̐�������
// --------------------------------
void EnemyManager::GenerateEnemy(const DirectX::SimpleMath::Vector3& position, EnemyType type)
{
	// �G�̐���
	switch (type)
	{
	case EnemyType::Goblin:	GenerateGoblin(position);	break;	// �S�u�����̐���
	case EnemyType::Boss:	GenerateBoss(position);		break;	// �{�X�̐���
	}
}


// --------------------------------
// �{�X�̃|�C���^���擾
// --------------------------------
Boss* EnemyManager::GetBossEnemy()
{
	// �{�X��T��
	for (auto& enemy : m_enemies)
	{
		if (enemy.type == EnemyType::Boss)
		{
			return dynamic_cast<Boss*>(enemy.data.get());
		}
	}

	return nullptr;
}

// --------------------------------
// �{�X�̍��W���擾
// --------------------------------
DirectX::SimpleMath::Vector3 EnemyManager::GetBossPosition()
{
	// �{�X��T��
	for (auto& enemy : m_enemies)
	{
		if (enemy.type == EnemyType::Boss)
		{
			return enemy.data->GetPosition();
		}
	}
	// �{�X�����Ȃ���Ό��_��Ԃ�
	return DirectX::SimpleMath::Vector3::Zero;
}


// --------------------------------
// �^�[�Q�b�g���̓G�̍��W���擾
// --------------------------------
DirectX::SimpleMath::Vector3 EnemyManager::GetPicupEnemyPosition()
{
	if (m_enemies.empty()) return DirectX::SimpleMath::Vector3::Zero;

	// �G�����Ȃ��ꍇ
	if (m_enemies.size() - 1 < m_targetEnemyIndex) m_targetEnemyIndex = 0;

	// �^�[�Q�b�g�̓G�̍��W���擾
	return m_enemies[m_targetEnemyIndex].data->GetPosition();
}



// --------------------------------
// �S�ẴS�u�����̍폜
// --------------------------------
void EnemyManager::DeleteAllGoblin()
{
	// �z��̗v�f�����S�ɍ폜���邽�߂�erase-remove�C�f�B�I�����g�p
	m_enemies.erase(std::remove_if(m_enemies.begin(),m_enemies.end(),[](EnemyData& enemy)
		{
			// �S�u�����̏ꍇ
			if (enemy.type == EnemyType::Goblin) 
				{
				// Finalize�����ƍ폜
				enemy.data->Finalize();
				return true;
				}	
			return false; // �폜���Ȃ�
		}),
		m_enemies.end()
	);
}


// --------------------------------
// �S�ẴS�u������HP��0�ɂ���
// --------------------------------
void EnemyManager::AllGoblinHPZero()
{
	// �S�u������HP��0�ɂ���
	for (auto& enemy : m_enemies)
	{
		if (enemy.type == EnemyType::Goblin)
		{
			// �ő�HP���̃_���[�W��S�̂ɗ^����
			enemy.data->GetHPSystem()->Damage(enemy.data->GetHPSystem()->GetHP());
		}
	}
}


// --------------------------------
// �G�P�̂̍폜
// --------------------------------
void EnemyManager::DeleteEnemy(IEnemy* enemy)
{
	// �Y������G��T��
	for (auto it = m_enemies.begin(); it != m_enemies.end(); ++it)
	{
		if (it->data.get() == enemy)
		{
			// �G�̏I������
			it->data->Finalize();
			// �z�񂩂�폜
			m_enemies.erase(it);
			return;
		}
	}
}


// --------------------------------
// �J�����̃^�[�Q�b�g��ύX
// --------------------------------
void EnemyManager::ChangeCameraTarget()
{
	// �^�[�Q�b�g�̃C���f�b�N�X��ύX
	m_targetEnemyIndex++;
	// �C���f�b�N�X���G�̐��𒴂�����
	if (m_targetEnemyIndex >= m_enemies.size())
	{
		// �퓬�ɖ߂�
		m_targetEnemyIndex = 0;
	}
}


// --------------------------------
// �G�������Ă��邩�̃t���O
// --------------------------------
bool EnemyManager::IsEnemysAlive()
{
	// �G�����Ȃ��ꍇ�N���A
	if (m_enemies.empty()) return false;


	// �{�X������ꍇ
	if (GetBossEnemy() != nullptr)
	{
		// �{�X��HP��0�ȉ��̏ꍇ
		if (GetBossEnemy()->GetHPSystem()->GetHP() <= 0)
		{
			// �o�ߎ��Ԃ����Z
			m_currentTime += m_elapsedTime;

			// �{�X������ł���̎��Ԃ���莞�Ԍo�߂�����false(���S)��Ԃ�
			return (m_currentTime <= BOSS_DEAD_DELAY);
		}
	}

	// �b�������Z�b�g
	m_currentTime = 0.0f;
	// ������
	return true;
}


// --------------------------------
// �S�Ă̓G�ɔ�_���\��ʒB
// --------------------------------
void EnemyManager::AllEnemyCanHit(void* flag)
{

	// �S�G��T��
	for (auto& enemy : m_enemies)
	{
		enemy.data->CanHit(*static_cast<bool*>(flag));
	}
}



// --------------------------------
// �S�u�����̐���
// --------------------------------
void EnemyManager::GenerateGoblin(const DirectX::SimpleMath::Vector3& position)
{
	// �S�u�����̐���
	auto goblin = Factory::CreateGoblin(m_playScene);
	// �ʒu�̐ݒ�
	goblin->SetPosition(position);
	// �z��Ɋi�[
	m_enemies.push_back(EnemyData{ EnemyType::Goblin, std::move(goblin)});
}


// --------------------------------
// �{�X�̐���
// --------------------------------
void EnemyManager::GenerateBoss(const DirectX::SimpleMath::Vector3& position)
{
	// �{�X�̐���
	auto boss = Factory::CreateBoss();
	// �ʒu�̐ݒ�
	boss->SetPosition(position);
	// �z��Ɋi�[
	m_enemies.push_back(EnemyData{ EnemyType::Boss, std::move(boss)});
}



// --------------------------------
// JSON�f�[�^����G�𐶐�����
// --------------------------------
void EnemyManager::GenerateEnemyFromJson()
{
	// json�t�@�C����ǂݍ���
	std::ifstream file(ENEMY_JSON_PATH);

	// �f�[�^��o�^
	auto jsonFile = nlohmann::json::parse(file);

	// �X�e�[�W�̔ԍ����擾
	std::string stageKey = "stage" + std::to_string(m_selectQuestIndex);

	// �X�e�[�W�L�[�����݂��Ȃ��ꍇ
	if (!jsonFile.contains(stageKey))
	{
		MessageBoxA(nullptr, ("�w�肳�ꂽ�X�e�[�W�f�[�^��������܂���: " + stageKey).c_str(), "�G���[", MB_OK);
		return;
	}


	for (const auto& enemyData : jsonFile[stageKey])
	{
		// �G�̃^�C�v���擾
		std::string type = enemyData["type"];
		EnemyType enemyType = (type == "Goblin") ? EnemyType::Goblin : EnemyType::Boss;

		// ���W���擾
		float x = enemyData["position"]["x"];
		float y = enemyData["position"]["y"];
		float z = enemyData["position"]["z"];
		DirectX::SimpleMath::Vector3 position(x, y, z);

		// �G�̐���
		GenerateEnemy(position, enemyType);
	}

	if (m_selectQuestIndex == 0)
	{
		// �`���[�g���A���p�ɃX�e�[�g��ύX����
		auto goblin = dynamic_cast<Goblin*>(m_enemies[0].data.get());
		goblin->ChangeState(goblin->GetTutorial());
	}
}
