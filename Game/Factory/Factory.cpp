#include "pch.h"
#include "Factory.h"
#include "../Messenger/KeyboardMessenger.h"		// キー入力メッセンジャー
#include "../Messenger/EventMessenger.h"		// イベントメッセンジャー

#include "../Scene/PlayScene.h"								// プレイシーン
#include "Interface/IObserver.h"							// オブザーバー
#include "../Camera/Camera.h"								// カメラ
#include "Effects/Particle.h"								// パーティクル
#include "Game/Sound/Sound.h"								// 音

#include "Libraries/MyLib/SkySphere.h"						// 天球
#include "../Stage/Floor/Floor.h"							// 床
#include "../Stage/Sea/Sea.h"								// 海
#include "../Stage/Wall/Wall.h"								// 壁
#include "../Player/Player.h"								// プレイヤー
#include "../Boss/Boss.h"									// 鬼（敵）
#include "../Goblin/Goblin.h"								// ゴブリン
#include "../Weapon/Sword/Sword.h"							// プレイヤーの武器
#include "../Weapon/Cudgel/Cudgel.h"						// 鬼（敵）の武器

#include "Libraries/MyLib/Collision/CollisionManager.h"		// 当たり判定統括
#include "../UI/!PlaySceneUIManager/PlaySceneUIManager.h"	// プレイシーンのUI
#include "Game/UI/Header/Warning.h"							// HP減ったときの警告UI
#include "../EnemyManager/EnemyManager.h"					// 敵マネージャー
#include "../Quest/QuestManager.h"							// クエストマネージャー

// ---------------------------------------------
// 当たり判定統括の生成関数
// ---------------------------------------------
std::unique_ptr<CollisionManager> Factory::CreateCollisionManager()
{
	// 当たり判定統括を宣言する
	std::unique_ptr<CollisionManager> collisionManager;
	collisionManager = std::make_unique<CollisionManager>();
	// 当たり判定統括の設定
	return collisionManager;
}


// ---------------------------------------------
// カメラの生成関数
// ---------------------------------------------
std::unique_ptr<Camera> Factory::CreateCamera()
{
	// カメラを宣言する
	std::unique_ptr<Camera> camera;
	camera = std::make_unique<Camera>();

	// カメラの設定
	return camera;
}


// ---------------------------------------------
// 天球の生成関数
// ---------------------------------------------
std::unique_ptr<SkySphere> Factory::CreateSkySphere()
{
	// 天球を宣言する
	std::unique_ptr<SkySphere> skySphere;
	skySphere = std::make_unique<SkySphere>();
	// 初期化処理
	skySphere->LoadSkySphereModel();
	// 天球の設定
	return skySphere;
}

// ---------------------------------------------
// パーティクルの生成関数
// ---------------------------------------------
std::unique_ptr<Particle> Factory::CreateParticle()
{
	// パーティクルを宣言する
	std::unique_ptr<Particle> particle;
	particle = std::make_unique<Particle>();
	// 初期化処理
	particle->Create();
	// パーティクルの設定
	return particle;
}


// ---------------------------------------------
// 床の生成関数
// ---------------------------------------------
std::unique_ptr<Floor> Factory::CreateFloor()
{
	// 床を宣言する
	std::unique_ptr<Floor> floor;
	floor = std::make_unique<Floor>();
	// 床の設定
	return floor;
}


// ---------------------------------------------
// 海の生成関数
// ---------------------------------------------
std::unique_ptr<Sea> Factory::CreateSea()
{
	std::unique_ptr<Sea> sea;
	sea = std::make_unique<Sea>();
	return sea;
}


// ---------------------------------------------
// 壁の生成関数
// ---------------------------------------------
std::unique_ptr<Wall> Factory::CreateWall(PlayScene* playScene)
{
	// 壁を宣言する
	std::unique_ptr<Wall> wall;
	wall = std::make_unique<Wall>(playScene);
	// 初期化処理
	wall->Initialize();
	// 壁の設定
	return wall;

}


// ---------------------------------------------
// プレイヤーの生成関数
// ---------------------------------------------
std::unique_ptr<Player> Factory::CreatePlayer(PlayScene* playScene)
{
	// プレイヤーを宣言する
	std::unique_ptr<Player> player;
	// プレイヤーを生成する
	player = std::make_unique<Player>(playScene);
	// 初期化処理
	player->Initialize();
	// キーを登録
	KeyboardMessenger::Attach(DirectX::Keyboard::X			, player.get(), KeyboardMessenger::KeyPressType::PRESSED);
	KeyboardMessenger::Attach(DirectX::Keyboard::LeftShift	, player.get(), KeyboardMessenger::KeyPressType::PRESSED);
	KeyboardMessenger::Attach(DirectX::Keyboard::Left		, player.get(), KeyboardMessenger::KeyPressType::DOWN);
	KeyboardMessenger::Attach(DirectX::Keyboard::Right		, player.get(), KeyboardMessenger::KeyPressType::DOWN);
	KeyboardMessenger::Attach(DirectX::Keyboard::Up			, player.get(), KeyboardMessenger::KeyPressType::DOWN);
	KeyboardMessenger::Attach(DirectX::Keyboard::Down		, player.get(), KeyboardMessenger::KeyPressType::DOWN);
	KeyboardMessenger::Attach(DirectX::Keyboard::L			, player.get(), KeyboardMessenger::KeyPressType::PRESSED);
	return player;
}



// ---------------------------------------------
// 鬼（敵）の生成関数
// ---------------------------------------------
std::unique_ptr<Boss> Factory::CreateBoss(PlayScene* playScene)
{
	// 鬼（敵）を宣言する
	std::unique_ptr<Boss> enemy;
	enemy = std::make_unique<Boss>(playScene);
	// 初期化処理
	enemy->Initialize();

	EventMessenger::Attach("canHit", std::bind(&Boss::CanHitSword, enemy.get()));
	EventMessenger::Attach("canNotHit",std::bind(&Boss::CanNotHitSword, enemy.get()));

	// 鬼（敵）の設定
	return enemy;
}


// ---------------------------------------------
// ゴブリンの生成関数
// ---------------------------------------------
std::unique_ptr<Goblin> Factory::CreateGoblin(PlayScene* playScene)
{
	// ゴブリンの宣言
	std::unique_ptr<Goblin> goblin;
	goblin = std::make_unique<Goblin>(playScene);
	// 初期化処理
	goblin->Initialize();
	// ゴブリンの設定
	return goblin;
}



// ---------------------------------------------
// プレイヤーの武器の生成関数
// ---------------------------------------------
std::unique_ptr<Sword> Factory::CreateSword(PlayScene* playScene)
{
	// プレイヤーの武器を宣言する
	std::unique_ptr<Sword> sword;
	sword = std::make_unique<Sword>(playScene);
	// 初期化処理
	sword->Initialize();
	// プレイヤーの武器の設定
	return sword;
}


// ---------------------------------------------
// 鬼（敵）の武器の生成関数
// ---------------------------------------------
std::unique_ptr<Cudgel> Factory::CreateCudgel(PlayScene* playScene)
{
	// 鬼（敵）の武器を宣言する
	std::unique_ptr<Cudgel> cudgel;
	cudgel = std::make_unique<Cudgel>(playScene);
	// 初期化処理
	cudgel->Initialize();
	// 鬼（敵）の武器の設定
	return cudgel;
}


// ---------------------------------------------
// プレイシーンのUIの生成関数
// ---------------------------------------------
std::unique_ptr<PlaySceneUIManager> Factory::CreateUIManager(PlayScene* playScene)
{
	// プレイシーンのUIを宣言する
	std::unique_ptr<PlaySceneUIManager> uiManager;
	uiManager = std::make_unique<PlaySceneUIManager>(playScene);
	// 初期化処理
	uiManager->Initialize();
	// プレイシーンのUIの設定
	return uiManager;
}


// ---------------------------------------------
// 敵マネージャーの生成関数
// ---------------------------------------------
std::unique_ptr<EnemyManager> Factory::CreateEnemyManager(PlayScene* playScene)
{
	// 敵マネージャーを宣言する
	std::unique_ptr<EnemyManager> enemyManager;
	enemyManager = std::make_unique<EnemyManager>(playScene);
	// 初期化処理
	enemyManager->Initialize(playScene);
	// 敵マネージャーの設定
	return enemyManager;
}


// ---------------------------------------------
// クエストマネージャーの生成関数
// ---------------------------------------------
std::unique_ptr<QuestManager> Factory::CreateQuestManager(PlayScene* playScene)
{
	// クエストマネージャーを宣言する
	std::unique_ptr<QuestManager> questManager;
	questManager = std::make_unique<QuestManager>(playScene);
	// 初期化処理
	questManager->InitializeQuest();
	// クエストマネージャーの設定
	return questManager;
}