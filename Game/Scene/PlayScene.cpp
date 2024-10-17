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

// ファクトリーメゾット ======================================
#include "Game/Factory/Factory.h"

// システム面 ================================================
#include "Game/Sound/BGM_Player.h"	// BGM再生

// オブジェクト関連　=========================================
#include "Game/Player/Player.h"			// プレイヤー
#include "Game/Enemy/Enemy.h"			// 鬼
#include "Game/Weapon/Sword/Sword.h"	// 刀
#include "Game/Weapon/Cudgel/Cudgel.h"	// 金棒

// ステージ関連 ==============================================
#include "Game/Stage/Floor/Floor.h"		// 床
#include "Game/Stage/Wall/Wall.h"		// 壁

// UI関連　====================================================
#include "Game/UI/PlaySceneUIManager/Header/PlaySceneUIManager.h"	// UI描画関連

// 当たり判定関連 =============================================
#include "Libraries/MyLib/Collision/CollisionManager.h"	// 当たり判定


#include"Interface/IObserver.h"
#include "Game/Observer/Messenger.h"

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


	// シーン変更フラグを初期化する
	m_isChangeScene = false;


	// オブジェクトの生成
	CreateObjects();
}


/// <summary>
/// factoryメゾットを用いて生成する関数
/// </summary>
void PlayScene::CreateObjects()
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();

	m_collisionManager	= Factory::CreateCollisionManager();	// パーティクル
	m_bgm				= Factory::CreateBGM_Player();			// BGM
	m_camera			= Factory::CreateCamera();				// カメラ
	m_skySphere			= Factory::CreateSkySphere(device);		// 天球	
	m_particles			= Factory::CreateParticle();			// パーティクル
	m_floor				= Factory::CreateFloor(device);			// フロア
	m_wall				= Factory::CreateWall(this);			// 壁
	m_player			= Factory::CreatePlayer(this);			// プレイヤ
	m_sword				= Factory::CreateSword(this);			// 刀
	m_enemy				= Factory::CreateEnemy(this); 			// 鬼
	m_cudgel			= Factory::CreateCudgel(this);			// 金棒
	m_uiManager			= Factory::CreateUIManager(this);		// UIマネージャ

	// 観察者リストをソートする
	Messenger::SortObserverList();
	// キー範囲リストを生成する
	Messenger::CreateKeyRangeList();
}



// すべてのキーの押下状態を検出する
inline bool IsKeyPress(DirectX::Keyboard::KeyboardStateTracker& stateTracker)
{
	// すべてのキーが押されたかどうかをチェック
	for (int key = 0; key < 256; key++)
	{
		// 特定のキーが押されているかを確認
		if (stateTracker.IsKeyPressed(static_cast<DirectX::Keyboard::Keys>(key)))
		{
			return true; // 押されたキーがあれば true を返す
		}
	}
	// どのキーも押されていない場合
	return false;
}


inline bool IsKeyDown(DirectX::Keyboard::State& state)
{
	// キーボードステートへのポインタを取得する
	auto ptr = reinterpret_cast<uint32_t*>(&state);
	for (int key = 0; key < 0xff; key++)
	{
		const unsigned int buffer = 1u << (key & 0x1f);
		// キーが押下げられたかどうかを調べる
		if (ptr[(key >> 5)] && buffer)	 return true;
	}
	// キーは押下げられていない
	return false;
}



/// <summary>
/// 更新関数
/// </summary>
/// <param name="elapsedTime">フレーム毎秒</param>
void PlayScene::Update(float elapsedTime)
{
	using namespace DirectX;
	UNREFERENCED_PARAMETER(elapsedTime);

	// キーボードの状態を取得する
	m_keyboardState = DirectX::Keyboard::Get().GetState();
	// キーボードステートトラッカーを更新する
	m_keyboardStateTracker.Update(m_keyboardState);

	// キーボードが押下げられたかどうかを判定する
	if (IsKeyDown	(m_keyboardState))			
	{
		Messenger::Notify(m_keyboardState); 
	}


	if (IsKeyPress(m_keyboardStateTracker))	
	{ 
		Messenger::Notify(m_keyboardStateTracker); 
	}

	// BGMの再生
	m_bgm->Update();

	// UIの更新
	m_uiManager->Update();

	// プレイヤーの更新処理
	m_player->Update(m_enemy->GetPosition(), elapsedTime);
	// プレイヤーの武器の更新処理
	m_sword->Update(elapsedTime);

	// 鬼の更新処理
	m_enemy->Update(elapsedTime);
	// 鬼の武器の更新処理
	m_cudgel->Update(elapsedTime);

	// カメラの回転行列の作成	引数にはプレイヤーの回転角を入れる
	SimpleMath::Matrix matrix = SimpleMath::Matrix::CreateRotationY( XMConvertToRadians ( m_player->GetAngle() ) );
	// カメラの更新
	m_camera->Update(m_player->GetPosition(), m_enemy->GetPosition(), matrix);

	// パーティクルの更新
	m_particles->Update(
		elapsedTime,
		m_player->GetPosition(),
		m_player->GetVelocity()
	);

	// 衝突判定の更新処理
	m_collisionManager->Update();

	// HPが0以下になったらゲーム終了
	if (m_enemy->GetEnemyHP()->GetHP() <= 0 || m_player->GetPlayerHP()->GetHP() <= 0)	m_isChangeScene = true;


#ifdef _DEBUG

#endif // _DEBUG
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

	//==== UI系の描画 ======================================================-------
	m_uiManager->Render();
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
