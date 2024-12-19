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
#include "../Scene/PlayScene.h"
#include "Interface/IEnemy.h"
#include "Game/CommonResources.h"

// ** �Œ�l **

// �S�u�����̃��f���p�X
const wchar_t* EnemyManager::GOBLIN_MODEL_PATH = L"Resources/Models/Oni/Body/oni.cmo";
// �{�X�̃��f���p�X
const wchar_t* EnemyManager::BOSS_MODEL_PATH = L"Resources/Models/Oni/Body/oni.cmo";

// ** �R���X�g���N�^ **
EnemyManager::EnemyManager()
{
}

// ** �f�X�g���N�^ **
EnemyManager::~EnemyManager()
{
}

// ** ���������� **
void EnemyManager::Initialize(PlayScene* playScene)
{
	m_goblinModel = CreateModel(GOBLIN_MODEL_PATH);
	m_bossModel = CreateModel(BOSS_MODEL_PATH);
}


// ** �X�V���� **
void EnemyManager::Update(float elapsedTime)
{
}


// ** �`�揈�� **
void EnemyManager::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
)
{
}


void EnemyManager::Finalize()
{
}


void EnemyManager::CreateGoblin(
	const DirectX::SimpleMath::Vector3 position
)
{
}



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