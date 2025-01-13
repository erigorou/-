// ---------------------------------------------
// �u���e�v�S�Ă̓G�̊Ǘ����s���N���X
// �u���t�v12/18
// �u�쐬�ҁv�r�c
// ---------------------------------------------

#pragma once
#include "pch.h"
#include "EnemyManager.h"
#include "../Enemy/Enemy.h"
#include "../Goblin/Goblin.h"
#include "../Factory/Factory.h"
#include "../Scene/PlayScene.h"
#include "Interface/IEnemy.h"
#include "Game/CommonResources.h"

// --------------------------------
// �Œ�l
// --------------------------------
// �S�u�����̃��f���p�X
const wchar_t* EnemyManager::GOBLIN_MODEL_PATH = L"Resources/Models/Oni/Body/oni.cmo";
// �{�X�̃��f���p�X
const wchar_t* EnemyManager::BOSS_MODEL_PATH = L"Resources/Models/Oni/Body/oni.cmo";


// --------------------------------
// �R���X�g���N�^
// --------------------------------
EnemyManager::EnemyManager(PlayScene* playScene)
	: m_targetEnemyIndex()
	, m_playScene(playScene)
{
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
	m_goblinModel = CreateModel(GOBLIN_MODEL_PATH);
	m_bossModel = CreateModel(BOSS_MODEL_PATH);


	GenerateBoss(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f));

	GenerateGoblin(DirectX::SimpleMath::Vector3(40.0f, 0.0f, 0.0f));
}


// --------------------------------
// �X�V����
// --------------------------------
void EnemyManager::Update(float elapsedTime)
{
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
	case EnemyType::Goblin:
		break;

	case EnemyType::Boss:
		break;
	}
}


// --------------------------------
// �{�X�̃|�C���^���擾
// --------------------------------
Enemy* EnemyManager::GetBossEnemy()
{
	// �{�X��T��
	for (auto& enemy : m_enemies)
	{
		if (enemy.type == EnemyType::Boss)
		{
			return dynamic_cast<Enemy*>(enemy.data.get());
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

	return DirectX::SimpleMath::Vector3::Zero;
}


// --------------------------------
// �^�[�Q�b�g���̓G�̍��W���擾
// --------------------------------
DirectX::SimpleMath::Vector3 EnemyManager::GetPicupEnemyPosition()
{
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
	auto boss = Factory::CreateEnemy(m_playScene);
	// �ʒu�̐ݒ�
	boss->SetPosition(position);
	// �z��Ɋi�[
	m_enemies.push_back(EnemyData{ EnemyType::Boss, std::move(boss)});
}


// --------------------------------
// ���f���̐�������
// --------------------------------
DirectX::Model* EnemyManager::CreateModel(const wchar_t* filePath)
{
	// �f�o�C�X�̎擾
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	// ���f���t�@�N�g���̐���
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// ���f���𐶐�
	return DirectX::Model::CreateFromCMO(device, filePath, *fx).get();
}