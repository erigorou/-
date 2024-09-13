/*
	@file	PlayScene.cpp
	@brief	プレイシーンクラス
*/
#include "pch.h"
#include "PlayScene.h"

#include <Model.h>
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugCamera.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/GridFloor.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"

// システム面 ================================================
#include "Game/Sound/BGM_Player.h"	// BGM再生

// オブジェクト関連　=========================================
#include "Game/Player/Player.h"					// プレイヤー
#include "Game/Enemy/Enemy.h"					// 鬼
#include "Game/Weapon/Sword/Sword.h"			// 刀
#include "Game/Weapon/Cudgel/Header/Cudgel.h"	// 金棒

// ステージ関連 ==============================================
#include "Game/Stage/Floor/Floor.h"				// 床
#include "Game/Stage/Wall/Wall.h"				// 壁


/// <summary>
/// コンストラクタ
/// </summary>
PlayScene::PlayScene()
	:
	m_commonResources{}
	,m_debugCamera{}
	,m_gridFloor{}
	,m_projection{}
	,m_isChangeScene{}
	,m_player{}
	,m_sword{}
	,m_enemy{}
	,m_cudgel{}
	,m_skySphere{}
	,m_particles{}
{
	m_commonResources = CommonResources::GetInstance();
}

/// <summary>
/// デストラクタ
/// </summary>
PlayScene::~PlayScene()
{
}

/// <summary>
/// 初期化関数
/// </summary>
void PlayScene::Initialize()
{
	using namespace DirectX;

	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();

	// グリッド床を作成する
	m_gridFloor = std::make_unique<mylib::GridFloor>(device, context, states);

	// デバッグカメラを作成する
	RECT rect{ m_commonResources->GetDeviceResources()->GetOutputSize() };

	m_debugCamera = std::make_unique<mylib::DebugCamera>();
	m_debugCamera->Initialize(rect.right, rect.bottom);

	// 射影行列を作成する
	m_projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 100000.0f
	);

	// TPSカメラを取得する
	m_camera = std::make_unique<Camera>();

	// シーン変更フラグを初期化する
	m_isChangeScene = false;

	// =================================================
	m_bgm = std::make_unique<BGM_Player>();
	m_bgm->InitializeFMOD("Resources/Sounds/BGM.ogg");

	// =================================================

	// 天球の生成とモデルの読み込み
	m_skySphere = std::make_unique<SkySphere>();
	m_skySphere->LoadSkySphereModel(device);

	// パーティクルの生成と初期化
	m_particles = std::make_unique<Particle>();
	m_particles->Create();

	// プレイヤーの生成と初期化
	m_player = std::make_unique<Player>(this);
	m_player->Initialize();

	// プレイヤーの武器の生成と初期化
	m_sword = std::make_unique<Sword>(this);
	m_sword->Initialize();

	// 鬼の生成と初期化
	m_enemy = std::make_unique<Enemy>(this);
	m_enemy->Initialize();

	// 鬼の武器を生成と初期化
	m_cudgel = std::make_unique<Cudgel>(this);
	m_cudgel->Initialize();

	// 床の作成と初期化
	m_floor = std::make_unique<Floor>(device);
	// 壁の生成と初期化
	m_wall = std::make_unique<Wall>();
	m_wall->Initialize();
}

/// <summary>
/// 更新関数
/// </summary>
/// <param name="elapsedTime">フレーム毎秒</param>
void PlayScene::Update(float elapsedTime)
{
	using namespace DirectX;
	UNREFERENCED_PARAMETER(elapsedTime);

	// BGMの再生
	m_bgm->Update();

	// プレイヤーの更新処理
	m_player->Update(m_enemy->GetPosition(), elapsedTime);
	// プレイヤーの武器の更新処理
	m_sword->Update(elapsedTime);

	// 鬼の更新処理
	m_enemy->Update(elapsedTime);
	// 鬼の武器の更新処理
	m_cudgel->Update(elapsedTime);

	// カメラの回転行列の作成	引数にはプレイヤーの回転角を入れる
	SimpleMath::Matrix matrix
		= SimpleMath::Matrix::CreateRotationY(
			XMConvertToRadians(m_player->GetAngle()));
	// カメラの更新
	m_camera->Update(m_player->GetPosition(), m_enemy->GetPosition(), matrix);

	// パーティクルの更新
	m_particles->Update(
		elapsedTime,
		m_player->GetPosition(),
		m_player->GetVelocity()
	);


	// HPが0以下になったらゲーム終了
	if (m_enemy->GetEnemyHP()->GetHP() <= 0)
	{
		m_isChangeScene = true;
	}
}


/// <summary>
/// 描画処理
/// </summary>
void PlayScene::Render()
{
	using namespace DirectX;

	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// ビュー行列を取得する
	const SimpleMath::Matrix& view = m_camera->GetViewMatrix();

	// ==== ステージの描画 =========================================================
	// 天球の描画
	m_skySphere->DrawSkySphere(context, states, view, m_projection);
	// 格子床を描画する
	m_gridFloor->Render(context, view, m_projection);
	// 床を描画する
	m_floor->Render(context, view, m_projection);
	// 壁を描画する
	m_wall->Render(context, states, view, m_projection);

	// === オブジェクトの描画 =======================================================
	// 敵の描画
	m_enemy->Render(device, context, states, view, m_projection);
	// 敵の武器の描画を行う
	m_cudgel->Render(device, context, states, view, m_projection);
	// プレイヤーの描画を行う
	m_player->Render(device, context, states, view, m_projection, m_commonResources);
	// プレイヤーの武器の描画を行う
	m_sword->Render(device, context, states, view, m_projection, m_commonResources);

	//==== エフェクト系の描画 ======================================================
	// パーティクルのビルボード作成
	m_particles->CreateBillboard(m_camera->GetTargetPosition(), m_camera->GetEyePosition(), DirectX::SimpleMath::Vector3::Up);
	// パーティクルの描画
	m_particles->Render(states, view, m_projection);
}

/// <summary>
/// 終了処理
/// </summary>
void PlayScene::Finalize()
{
	m_bgm->FinalizeFMOD();
}
/// <summary>
/// 次のシーンIDの取得関数
/// </summary>
/// <returns>リザルト or 何も返さない</returns>
IScene::SceneID PlayScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		return IScene::SceneID::RESULT;
	}

	// シーン変更がない場合
	return IScene::SceneID::NONE;
}
