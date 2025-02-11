// ---------------------------------------------
// 名前:	EnemyManager.cpp
// 内容:	全ての敵の管理を行うクラス
//			すべての敵の生成、更新、描画、削除を行う
// 作成:	池田
// ---------------------------------------------
#pragma once
// インクルード
#include "pch.h"
#include "Game/Data/GameData.h"
#include "EnemyManager.h"
#include "Game/Messenger/EventMessenger.h"
#include "../Boss/Boss.h"
#include "../Goblin/Goblin.h"
#include "../Factory/Factory.h"
#include "Interface/IEnemy.h"
#include "nlohmann/json.hpp"
#include <fstream>

// --------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="playScene"></param>
// --------------------------------
EnemyManager::EnemyManager()
	: 
	m_targetEnemyIndex(),
	m_elapsedTime(),
	m_currentTime()
{
	// セレクトクエストのインデックスを取得
	m_selectQuestIndex = GameData::GetInstance()->GetSelectStage();

	// Jsonファイルから敵を生成
	GenerateEnemyFromJson();
}

// --------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// --------------------------------
EnemyManager::~EnemyManager()
{
	// 敵すべての終了処理
	for (auto& enemy : m_enemies)
	{
		enemy.data->Finalize();
	}

	// イベントメッセンジャーから削除
	EventMessenger::Detach(EventList::EnemyCanHit);
	EventMessenger::Detach(EventList::DeleteAllGoblin);
	EventMessenger::Detach(EventList::DeleteEnemy);

	// クリア
	m_enemies.clear();
}


// --------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// --------------------------------
void EnemyManager::Initialize()
{
	// 敵に攻撃ができるイベントを登録
	EventMessenger::Attach(EventList::EnemyCanHit, std::bind(&EnemyManager::AllEnemyCanHit, this, std::placeholders::_1));
	// 全ての小鬼を削除するイベントを登録
	EventMessenger::Attach(EventList::DeleteAllGoblin, std::bind(&EnemyManager::AllGoblinHPZero, this));
	// 敵1体を削除するイベントを登録
	EventMessenger::Attach(EventList::DeleteEnemy, std::bind(&EnemyManager::DeleteEnemy, this, std::placeholders::_1));
	// カメラのターゲットポイントを入手するイベントを登録
	EventMessenger::AttachGetter(GetterList::GetTargetPosition, std::bind(&EnemyManager::GetPicupEnemyPosition, this));
}

// --------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// --------------------------------
void EnemyManager::Update(float elapsedTime)
{
	// 経過時間を保存
	m_elapsedTime = elapsedTime;

	// 敵がいない場合は処理なし
	if (m_enemies.empty()) return;

	// すべての敵の更新処理
	for (auto& enemy : m_enemies)
	{
		if(enemy.data == nullptr) continue;

		enemy.data->Update(elapsedTime);
	}
}

// --------------------------------
/// <summary>
/// 描画処理
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="projection">プロジェクション行列</param>
// --------------------------------
void EnemyManager::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
)
{
	if (m_enemies.empty()) return;

	// ゴブリン、ボスの描画
	for (auto& enemy : m_enemies)
	{
		enemy.data->Render(view, projection);
	}
}

// --------------------------------
/// <summary>
/// 終了処理
/// </summary>
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
/// <summary>
/// 敵の生成処理
/// </summary>
/// <param name="position">座標</param>
/// <param name="type">種類</param>
// --------------------------------
void EnemyManager::GenerateEnemy(const DirectX::SimpleMath::Vector3& position, EnemyType type)
{
	// 敵の生成
	switch (type)
	{
	case EnemyType::Goblin:	GenerateGoblin(position);	break;	// ゴブリンの生成
	case EnemyType::Boss:	GenerateBoss(position);		break;	// ボスの生成
	}
}

// --------------------------------
/// <summary>
/// ボスの取得
/// </summary>
/// <returns>ボスのポインタ</returns>
// --------------------------------
Boss* EnemyManager::GetBossEnemy()
{
	// ボスを探索
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
/// <summary>
/// ボスの座標を取得
/// </summary>
/// <returns>ボスの座標</returns>
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
	// ボスが入なければ原点を返す
	return DirectX::SimpleMath::Vector3::Zero;
}

// --------------------------------
/// <summary>
/// ターゲット中の座標を取得
/// </summary>
/// <returns>ターゲット中の座標</returns>
// --------------------------------
void* EnemyManager::GetPicupEnemyPosition()
{
	// ターゲットの敵の座標を取得(敵がいない場合)
	m_targetPosition = DirectX::SimpleMath::Vector3{ 0.0f, 0.0f, 200.0f };
	// 敵がいない場合はそのまま返す
	if (m_enemies.empty()) return &m_targetPosition;
	// 敵の配列をオーバーしている場合は0番を見る
	if (m_enemies.size() - 1 < m_targetEnemyIndex) m_targetEnemyIndex = 0;
	// ターゲットの敵の座標を取得	
	m_targetPosition = m_enemies[m_targetEnemyIndex].data->GetPosition();
	// 座標を返す
	return &m_targetPosition;
}

// --------------------------------
/// <summary>
/// 全ゴブリンのHPを0にする
/// </summary>
// --------------------------------
void EnemyManager::AllGoblinHPZero()
{
	// ゴブリンのHPを0にする
	for (auto& enemy : m_enemies)
	{
		if (enemy.type == EnemyType::Goblin)
		{
			// 最大HP分のダメージを全体に与える
			enemy.data->GetHPSystem()->Damage(enemy.data->GetHPSystem()->GetHP());
		}
	}
}

// ---------------------------------------------
/// <summary>
/// 敵1体を削除する処理
/// </summary>
/// <param name="pointer">削除する対象のポインタ</param>
// ---------------------------------------------
void EnemyManager::DeleteEnemy(void* pointer)
{
	// ポインタをIEnemy型にキャスト
	auto enemy = static_cast<IEnemy*>(pointer);

	// 敵の削除
	m_enemies.erase(
		std::remove_if(m_enemies.begin(), m_enemies.end(),
			[enemy](const EnemyData& data) {
				if (data.data.get() == enemy)
				{
					data.data->Finalize();  // 敵の終了処理
					return true;            // 削除対象とする
				}
				return false;               // 削除しない
			}),
		m_enemies.end() // 配列の末尾を削除範囲として指定
	);
}


// --------------------------------
/// <summary>
/// カメラのターゲットを変更
/// </summary>
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
/// <summary>
/// 敵が生存しているかの判定
/// </summary>
/// <returns>生存：true</returns>
// --------------------------------
bool EnemyManager::IsEnemysAlive()
{
	// 敵がいない場合クリア
	if (m_enemies.empty()) return false;

	// ボスがいる場合
	if (GetBossEnemy() != nullptr)
	{
		// ボスのHPが0以下の場合
		if (GetBossEnemy()->GetHPSystem()->GetHP() <= 0)
		{
			// 経過時間を加算
			m_currentTime += m_elapsedTime;

			// ボスが死んでからの時間が一定時間経過したらfalse(死亡)を返す
			return (m_currentTime <= BOSS_DEAD_DELAY);
		}
	}

	// 秒数をリセット
	m_currentTime = 0.0f;
	// 生存中
	return true;
}

// --------------------------------
/// <summary>
/// 全敵に攻撃可能フラグを設定
/// </summary>
/// <param name="flag">設定するフラグ</param>
// --------------------------------
void EnemyManager::AllEnemyCanHit(void* flag)
{
	// 全敵を探索
	for (auto& enemy : m_enemies)
	{
		enemy.data->CanHit(*static_cast<bool*>(flag));
	}
}

// --------------------------------
/// <summary>
/// ゴブリンの生成
/// </summary>
/// <param name="position">座標</param>
// --------------------------------
void EnemyManager::GenerateGoblin(const DirectX::SimpleMath::Vector3& position)
{
	// ゴブリンの生成
	auto goblin = Factory::CreateGoblin();
	// 位置の設定
	goblin->SetPosition(position);
	// 配列に格納
	m_enemies.push_back(EnemyData{ EnemyType::Goblin, std::move(goblin) });
}

// --------------------------------
/// <summary>
/// ボスの生成
/// </summary>
/// <param name="position">座標</param>
// --------------------------------
void EnemyManager::GenerateBoss(const DirectX::SimpleMath::Vector3& position)
{
	// ボスの生成
	auto boss = Factory::CreateBoss();
	// 位置の設定
	boss->SetPosition(position);
	// 配列に格納
	m_enemies.push_back(EnemyData{ EnemyType::Boss, std::move(boss) });
}

// --------------------------------
/// <summary>
/// Jsonファイルデータで敵を生成
/// </summary>
// --------------------------------
void EnemyManager::GenerateEnemyFromJson()
{
	// jsonファイルを読み込む
	std::ifstream file(ENEMY_JSON_PATH);

	// データを登録
	auto jsonFile = nlohmann::json::parse(file);

	// ステージの番号を取得
	std::string stageKey = "stage" + std::to_string(m_selectQuestIndex);

	// ステージキーが存在しない場合
	if (!jsonFile.contains(stageKey))
	{
		MessageBoxA(nullptr, ("指定されたステージデータが見つかりません: " + stageKey).c_str(), "エラー", MB_OK);
		return;
	}

	for (const auto& enemyData : jsonFile[stageKey])
	{
		// 敵のタイプを取得
		std::string type = enemyData["type"];
		EnemyType enemyType = (type == "Goblin") ? EnemyType::Goblin : EnemyType::Boss;

		// 座標を取得
		float x = enemyData["position"]["x"];
		float y = enemyData["position"]["y"];
		float z = enemyData["position"]["z"];
		DirectX::SimpleMath::Vector3 position(x, y, z);

		// 敵の生成
		GenerateEnemy(position, enemyType);
	}

	if (m_selectQuestIndex == 0)
	{
		// チュートリアル用にステートを変更する
		auto goblin = dynamic_cast<Goblin*>(m_enemies[0].data.get());
		goblin->ChangeState(GoblinState::TUTORIAL);

		// チュートリアル用にステートを変更する
		goblin = dynamic_cast<Goblin*>(m_enemies[1].data.get());
		goblin->ChangeState(GoblinState::TUTORIAL);
	}
}