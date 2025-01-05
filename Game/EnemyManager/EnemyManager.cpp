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
#include "../Factory/Factory.h"
#include "../Scene/PlayScene.h"
#include "Interface/IEnemy.h"
#include "Game/CommonResources.h"

// --------------------------------
// 固定値
// --------------------------------
// ゴブリンのモデルパス
const wchar_t* EnemyManager::GOBLIN_MODEL_PATH = L"Resources/Models/Oni/Body/oni.cmo";
// ボスのモデルパス
const wchar_t* EnemyManager::BOSS_MODEL_PATH = L"Resources/Models/Oni/Body/oni.cmo";


// --------------------------------
// コンストラクタ
// --------------------------------
EnemyManager::EnemyManager(PlayScene* playScene)
	: m_targetEnemyIndex()
	, m_playScene(playScene)
{
}


// --------------------------------
// デストラクタ
// --------------------------------
EnemyManager::~EnemyManager()
{
}


// --------------------------------
// 初期化処理
// --------------------------------
void EnemyManager::Initialize(PlayScene* playScene)
{
	m_goblinModel = CreateModel(GOBLIN_MODEL_PATH);
	m_bossModel = CreateModel(BOSS_MODEL_PATH);

	// 試しに1体生成する
	GenerateGoblin(DirectX::SimpleMath::Vector3(40.0f, 0.0f, 0.0f));
}


// --------------------------------
// 更新処理
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
// 描画処理
// --------------------------------
void EnemyManager::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	if (m_enemies.empty()) return;

	// ゴブリン、ボスの描画
	for (auto& enemy : m_enemies)
	{
		enemy.data->Render(view, projection);
	}
}


// --------------------------------
// 終了処理
// --------------------------------
void EnemyManager::Finalize()
{
	if (m_enemies.empty()) return;

	// ゴブリン、ボスの解放
	for (auto& enemy : m_enemies)
	{
		enemy.data->Finalize();
	}
	m_enemies.clear();
}

// --------------------------------
// 敵の生成処理
// --------------------------------
void EnemyManager::GenerateEnemy(const DirectX::SimpleMath::Vector3& position, EnemyType type)
{
	// 敵の生成
	switch (type)
	{
	case EnemyType::Goblin:
		break;

	case EnemyType::Boss:
		break;
	}
}




// --------------------------------
// 全てのゴブリンの削除
// --------------------------------
void EnemyManager::DeleteAllGoblin()
{
	// 全ての敵を探索
	for (auto& Enemy : m_enemies)
	{
		// ゴブリンなら削除
		if (Enemy.type == EnemyType::Goblin)
		{
			Enemy.data->Finalize();
		}
	}
}

// --------------------------------
// カメラのターゲットを変更
void EnemyManager::ChangeCameraTarget()
{
	// ターゲットのインデックスを変更
	m_targetEnemyIndex++;
	// インデックスが敵の数を超えたら
	if (m_targetEnemyIndex >= m_enemies.size())
	{
		// 戦闘に戻る
		m_targetEnemyIndex = 0;
	}
}



void EnemyManager::GenerateGoblin(const DirectX::SimpleMath::Vector3& position)
{
	// ゴブリンの生成
	auto goblin = Factory::CreateGoblin(m_playScene);
	// 位置の設定
	goblin->SetPosition(position);
	// 配列に格納
	m_enemies.push_back(EnemyData{ EnemyType::Goblin, std::move(goblin)});
}


void EnemyManager::GenerateBoss(const DirectX::SimpleMath::Vector3& position)
{
	// ボスの生成
	auto boss = Factory::CreateEnemy(m_playScene);
	// 位置の設定
	boss->SetPosition(position);
	// 配列に格納
	m_enemies.push_back(EnemyData{ EnemyType::Boss, std::move(boss)});
}

// --------------------------------
// モデルの生成処理
// --------------------------------
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