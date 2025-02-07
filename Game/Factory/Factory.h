// ---------------------------------------------
// 名前:	Factory.cpp
// 内容:	オブジェクトの生成・初期化を行うクラス
//			使用するキーもここで登録する
// 作成:	池田
// ---------------------------------------------
#pragma once
#ifndef FACTORY
#define FACTORY
// インクルード
#include "pch.h"

// 前方宣言
class PlayScene;
class IObserver;
class CollisionManager;
class Sound;
class Camera;
class SkySphere;
class Particle;
class Floor;
class Sea;
class Wall;
class Player;
class Boss;
class Goblin;
class Sword;
class Cudgel;
class Warning;
class PlaySceneUIManager;
class EnemyManager;
class QuestManager;

/// <summary>
/// オブジェクトの生成クラス
/// </summary>
class Factory
{
// ------------------------------------------------
// メンバ関数(公開)
// ------------------------------------------------
public:
	// カメラの生成
	static std::unique_ptr<Camera> CreateCamera();
	// パーティクルの生成
	static std::unique_ptr<Particle> CreateParticle();
	// 天球の生成
	static std::unique_ptr<SkySphere> CreateSkySphere();
	// 床の生成
	static std::unique_ptr<Floor> CreateFloor();
	// 海の生成
	static std::unique_ptr<Sea> CreateSea();
	// 壁の生成
	static std::unique_ptr<Wall> CreateWall();

	// プレイヤーの生成
	static std::unique_ptr<Player> CreatePlayer(PlayScene* playScene);
	// ボスの生成
	static std::unique_ptr<Boss> CreateBoss();
	// 小鬼の生成
	static std::unique_ptr<Goblin> CreateGoblin();
	// 刀の生成
	static std::unique_ptr<Sword> CreateSword(Player* player);
	// 金棒の生成
	static std::unique_ptr<Cudgel> CreateCudgel(Boss* boss);

	// 衝突判定マネージャーの作成
	static std::unique_ptr<CollisionManager> CreateCollisionManager();
	// UIマネージャーの作成
	static std::unique_ptr<PlaySceneUIManager> CreateUIManager(PlayScene* playScene);
	// 敵マネージャーの作成
	static std::unique_ptr<EnemyManager>		CreateEnemyManager();
	// クエストマネージャーの作成
	static std::unique_ptr<QuestManager>		CreateQuestManager(PlayScene* playScene);
};

#endif // !FACTORY
