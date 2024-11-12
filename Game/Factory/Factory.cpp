#include "pch.h"
#include "Factory.h"
#include "../Observer/Messenger.h"

#include "../Scene/PlayScene.h"										// プレイシーン
#include "Interface/IObserver.h"									// オブザーバー
#include "Libraries/MyLib/Collision/CollisionManager.h"				// 当たり判定統括
#include "../Camera/Camera.h"										// カメラ
#include "Libraries/MyLib/SkySphere.h"								// 天球
#include "Effects/Particle.h"										// パーティクル
#include "../Stage/Floor/Floor.h"									// 床
#include "../Stage/Wall/Wall.h"										// 壁
#include "../Player/Player.h"										// プレイヤー
#include "../Enemy/Enemy.h"											// 鬼（敵）
#include "../Weapon/Sword/Sword.h"									// プレイヤーの武器
#include "../Weapon/Cudgel/Cudgel.h"								// 鬼（敵）の武器
#include "../UI/!PlaySceneUIManager/PlaySceneUIManager.h"			// プレイシーンのUI
#include "Game/UI/Header/Warning.h"									// HP減ったときの警告UI

using namespace DirectX;


// 当たり判定統括の生成関数
std::unique_ptr<CollisionManager> Factory::CreateCollisionManager()
{
	// 当たり判定統括を宣言する
	std::unique_ptr<CollisionManager> collisionManager;
	collisionManager = std::make_unique<CollisionManager>();
	// 当たり判定統括の設定
	return collisionManager;
}


// カメラの生成関数
std::unique_ptr<Camera> Factory::CreateCamera()
{
	// カメラを宣言する
	std::unique_ptr<Camera> camera;
	camera = std::make_unique<Camera>();
	// カメラの設定
	return camera;
}


// 天球の生成関数
std::unique_ptr<SkySphere> Factory::CreateSkySphere(ID3D11Device1* device)
{
	// 天球を宣言する
	std::unique_ptr<SkySphere> skySphere;
	skySphere = std::make_unique<SkySphere>();
	// 初期化処理
	skySphere->LoadSkySphereModel(device);
	// 天球の設定
	return skySphere;
}


// パーティクルの生成関数
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


// 床の生成関数
std::unique_ptr<Floor> Factory::CreateFloor()
{
	// 床を宣言する
	std::unique_ptr<Floor> floor;
	floor = std::make_unique<Floor>();
	// 床の設定
	return floor;
}


// 壁の生成関数
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


// プレイヤーの生成関数
std::unique_ptr<Player> Factory::CreatePlayer(PlayScene* playScene)
{
	// プレイヤーを宣言する
	std::unique_ptr<Player> player;
	// プレイヤーを生成する
	player = std::make_unique<Player>(playScene);
	// 初期化処理
	player->Initialize();
	// キーを登録
	Messenger::Attach(DirectX::Keyboard::X			, player.get(), Messenger::KeyPressType::PRESSED);
	Messenger::Attach(DirectX::Keyboard::LeftShift	, player.get(), Messenger::KeyPressType::PRESSED);
	Messenger::Attach(DirectX::Keyboard::Left		, player.get(), Messenger::KeyPressType::DOWN);
	Messenger::Attach(DirectX::Keyboard::Right		, player.get(), Messenger::KeyPressType::DOWN);
	Messenger::Attach(DirectX::Keyboard::Up			, player.get(), Messenger::KeyPressType::DOWN);
	Messenger::Attach(DirectX::Keyboard::Down		, player.get(), Messenger::KeyPressType::DOWN);
	// プレイヤーの設定
	return player;
}



// 鬼（敵）の生成関数
std::unique_ptr<Enemy> Factory::CreateEnemy(PlayScene* playScene)
{
	// 鬼（敵）を宣言する
	std::unique_ptr<Enemy> enemy;
	enemy = std::make_unique<Enemy>(playScene);
	// 初期化処理
	enemy->Initialize();
	// 鬼（敵）の設定
	return enemy;
}


// プレイヤーの武器の生成関数
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


// 鬼（敵）の武器の生成関数
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


// プレイシーンのUIの生成関数
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
