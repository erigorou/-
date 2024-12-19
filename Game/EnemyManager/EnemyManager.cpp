// ---------------------------------------------
// 「内容」全ての敵の管理を行うクラス
// 「日付」12/18
// 「作成者」池田
// ---------------------------------------------

#pragma once
#include "pch.h"
#include "EnemyManager.h"
#include "../Enemy/Enemy.h"
#include "../Goblin/Goblin.h"
#include "../Scene/PlayScene.h"
#include "Interface/IEnemy.h"
#include "Game/CommonResources.h"

// ** 固定値 **

// ゴブリンのモデルパス
const wchar_t* EnemyManager::GOBLIN_MODEL_PATH = L"Resources/Models/Oni/Body/oni.cmo";
// ボスのモデルパス
const wchar_t* EnemyManager::BOSS_MODEL_PATH = L"Resources/Models/Oni/Body/oni.cmo";

// ** コンストラクタ **
EnemyManager::EnemyManager()
{
}

// ** デストラクタ **
EnemyManager::~EnemyManager()
{
}

// ** 初期化処理 **
void EnemyManager::Initialize(PlayScene* playScene)
{
	m_goblinModel = CreateModel(GOBLIN_MODEL_PATH);
	m_bossModel = CreateModel(BOSS_MODEL_PATH);
}


// ** 更新処理 **
void EnemyManager::Update(float elapsedTime)
{
}


// ** 描画処理 **
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
	// デバイスの取得
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	// モデルファクトリの生成
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// モデルを生成
	return DirectX::Model::CreateFromCMO(device, filePath, *fx).get();
}