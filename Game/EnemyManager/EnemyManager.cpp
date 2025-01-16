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
	, m_elapsedTime()
	, m_currentTime()
{
	// �Z���N�g�X�e�[�W�̎擾
	m_selectQuestIndex = GameData::GetInstance()->GetSelectStage();
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

	m_goblinModel = CreateModel(GOBLIN_MODEL_PATH);
	m_bossModel = CreateModel(BOSS_MODEL_PATH);

	// �����G�̐���
	GenerateStartEnemy();
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

// --------------------------------
// �����G�̐���
// --------------------------------
void EnemyManager::GenerateStartEnemy()
{
	switch (m_selectQuestIndex)
	{
	case 0:
		GenerateEnemy0();
		break;
	case 1:
		GenerateEnemy1();
		break;
	case 2:
		GenerateEnemy2();

	default:
		// �G���[���b�Z�[�W
		MessageBox(nullptr, L"�G�̐����Ɏ��s���܂���", L"�G���[", MB_OK);
		break;
	}
}


// --------------------------------
// �N�G�X�g0�̓G���� ()
// --------------------------------
void EnemyManager::GenerateEnemy0()
{
	// �G�̐���
	GenerateEnemy(DirectX::SimpleMath::Vector3(-20.0f, 0.0f, -40.0f), EnemyType::Goblin);

	// �`���[�g���A���p�ɃX�e�[�g��ύX����
	auto goblin = dynamic_cast<Goblin*>(m_enemies[0].data.get());
	goblin->ChangeState(goblin->GetTutorial());
}


// --------------------------------
// �N�G�X�g1�̓G����
// --------------------------------
void EnemyManager::GenerateEnemy1()
{
	// ����
	GenerateEnemy(DirectX::SimpleMath::Vector3(0.0f, 0.0f, -40.0f), EnemyType::Boss);
	GenerateEnemy(DirectX::SimpleMath::Vector3(-80.0f, 0.0f, -40.0f), EnemyType::Goblin);
}


// --------------------------------
// �N�G�X�g2�̓G����
// --------------------------------
void EnemyManager::GenerateEnemy2()
{
	// �S�u�����̐���
	GenerateEnemy(DirectX::SimpleMath::Vector3(0.0f, 0.0f, -40.0f), EnemyType::Boss);
}

