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


	GenerateBoss(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f));

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
// ボスのポインタを取得
// --------------------------------
Enemy* EnemyManager::GetBossEnemy()
{
	// ボスを探索
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
// ボスの座標を取得
// --------------------------------
DirectX::SimpleMath::Vector3 EnemyManager::GetBossPosition()
{
	// ボスを探索
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
// ターゲット中の敵の座標を取得
// --------------------------------
DirectX::SimpleMath::Vector3 EnemyManager::GetPicupEnemyPosition()
{
	// 敵がいない場合
	if (m_enemies.size() - 1 < m_targetEnemyIndex) m_targetEnemyIndex = 0;

	// ターゲットの敵の座標を取得
	return m_enemies[m_targetEnemyIndex].data->GetPosition();
}



// --------------------------------
// 全てのゴブリンの削除
// --------------------------------
void EnemyManager::DeleteAllGoblin()
{
	// 配列の要素を安全に削除するためにerase-removeイディオムを使用
	m_enemies.erase(std::remove_if(m_enemies.begin(),m_enemies.end(),[](EnemyData& enemy)
		{
			// ゴブリンの場合
			if (enemy.type == EnemyType::Goblin) 
				{
				// Finalize処理と削除
				enemy.data->Finalize();
				return true;
				}	
			return false; // 削除しない
		}),
		m_enemies.end()
	);
}


// --------------------------------
// 敵１体の削除
// --------------------------------
void EnemyManager::DeleteEnemy(IEnemy* enemy)
{
	// 該当する敵を探索
	for (auto it = m_enemies.begin(); it != m_enemies.end(); ++it)
	{
		if (it->data.get() == enemy)
		{
			// 敵の終了処理
			it->data->Finalize();
			// 配列から削除
			m_enemies.erase(it);
			return;
		}
	}
}


// --------------------------------
// カメラのターゲットを変更
// --------------------------------
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


// --------------------------------
// ゴブリンの生成
// --------------------------------
void EnemyManager::GenerateGoblin(const DirectX::SimpleMath::Vector3& position)
{
	// ゴブリンの生成
	auto goblin = Factory::CreateGoblin(m_playScene);
	// 位置の設定
	goblin->SetPosition(position);
	// 配列に格納
	m_enemies.push_back(EnemyData{ EnemyType::Goblin, std::move(goblin)});
}


// --------------------------------
// ボスの生成
// --------------------------------
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