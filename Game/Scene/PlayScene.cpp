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
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"

// ファクトリーメゾット ======================================
#include "Game/Factory/Factory.h"

// システム面 ================================================
#include "Game/Sound/Sound.h"	// 音

// オブジェクト関連　=========================================
#include "Game/Player/Player.h"			// プレイヤー
#include "Game/Enemy/Enemy.h"			// 鬼
#include "Game/Weapon/Sword/Sword.h"	// 刀
#include "Game/Weapon/Cudgel/Cudgel.h"	// 金棒

// ステージ関連 ==============================================
#include "Game/Stage/Floor/Floor.h"		// 床
#include "Game/Stage/Wall/Wall.h"		// 壁

// UI関連　====================================================
#include "Game/UI/!PlaySceneUIManager/PlaySceneUIManager.h"	// UI描画関連

// 当たり判定関連 =============================================
#include "Libraries/MyLib/Collision/CollisionManager.h"	// 当たり判定

// ゲームデータ ===============================================
#include "Game/Data/GameData.h"

// Sound　======================================================
#include "Game/Sound/Sound.h"
#include"Interface/IObserver.h"
#include "Game/Observer/Messenger.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayScene::PlayScene()
	:
	m_commonResources{}
	,m_debugCamera{}
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
	GameData::GetInstance()->SetBattleResult(GameData::BATTLE_RESULT::NONE);
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

	Messenger::Clear();	// メッセンジャーのクリア

	m_collisionManager	= Factory::CreateCollisionManager	();			// パーティクル
	m_camera			= Factory::CreateCamera				();			// カメラ
	m_skySphere			= Factory::CreateSkySphere			(device);	// 天球	
	m_particles			= Factory::CreateParticle			();			// パーティクル
	m_floor				= Factory::CreateFloor				(device);	// フロア
	m_wall				= Factory::CreateWall				(this);		// 壁
	m_player			= Factory::CreatePlayer				(this);		// プレイヤ
	m_sword				= Factory::CreateSword				(this);		// 刀
	m_enemy				= Factory::CreateEnemy				(this); 	// 鬼
	m_cudgel			= Factory::CreateCudgel				(this);		// 金棒
	m_uiManager			= Factory::CreateUIManager			(this);		// UIマネージャ

	// 観察者リストをソートする
	Messenger::SortObserverList();
	// キー範囲リストを生成する
	Messenger::CreateKeyRangeList();

	// カメラをプレイシーンで設定
	m_camera->ChangeState(m_camera->GetPlayState());

	// BGM変更
	Sound::GetInstance()->ChangeBGM(Sound::BGM_TYPE::PLAY);
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
	if (IsKeyDown	(m_keyboardState)		)	Messenger::Notify(m_keyboardState		); 
	if (IsKeyPress	(m_keyboardStateTracker))	Messenger::Notify(m_keyboardStateTracker); 

	// UIの更新
	m_uiManager->Update(elapsedTime);

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
	m_camera->Update(m_player->GetPosition(), m_enemy->GetPosition(), matrix, elapsedTime);

	// パーティクルの更新
	m_particles->Update(
		elapsedTime,
		m_player->GetPosition(),
		m_player->GetVelocity()
	);

	// 衝突判定の更新処理
	m_collisionManager->Update();

	// 勝敗を決める
	CheckResult();


#ifdef _DEBUG
#endif // _DEBUG
}


/// <summary>
/// 描画処理
/// </summary>
void PlayScene::Render()
{
	using namespace DirectX;

	// ビュー行列を取得する
	const SimpleMath::Matrix& view = m_camera->GetViewMatrix();

	m_collisionManager->Render(view, m_projection);

	// ==== ステージの描画 =========================================================
	// 天球の描画
	m_skySphere->DrawSkySphere(view, m_projection);
	// 床を描画する
	m_floor->Render(view, m_projection);
	// 壁を描画する
	m_wall->Render(view, m_projection);

	// === オブジェクトの描画 =======================================================
	// 敵の描画
	m_enemy->Render(view, m_projection);
	// 敵の武器の描画を行う
	m_cudgel->Render(view, m_projection);
	// プレイヤーの描画を行う
	m_player->Render(view, m_projection);
	// プレイヤーの武器の描画を行う
	m_sword->Render(view, m_projection);

	//==== エフェクト系の描画 ======================================================
	// パーティクルのビルボード作成
	m_particles->CreateBillboard(m_camera->GetTargetPosition(), m_camera->GetEyePosition(), DirectX::SimpleMath::Vector3::Up);
	// パーティクルの描画
	m_particles->Render(view, m_projection);

	//==== UI系の描画 ======================================================-------
	m_uiManager->Render();
}

/// <summary>
/// 終了処理
/// </summary>
void PlayScene::Finalize()
{
}

/// <summary>
/// カメラを揺らす
/// </summary>
void PlayScene::SetShakeCamera()
{
	m_camera->SetShake();
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


void PlayScene::CheckResult()
{
	auto data = GameData::GetInstance();

	// 敵が死亡
	if (m_enemy->GetEnemyHP()->GetHP() <= 0)
	{
		m_isChangeScene = true;
		data->SetBattleResult(GameData::BATTLE_RESULT::WIN);
	}
	// プレイヤーが死亡
	else if (m_player->GetPlayerHP()->GetHP() <= 0)
	{
		m_isChangeScene = true;
		data->SetBattleResult(GameData::BATTLE_RESULT::LOSE);
	}


#ifdef _DEBUG
	if (m_keyboardStateTracker.IsKeyPressed(DirectX::Keyboard::Keys::F7))
	{
		m_isChangeScene = true;
		data->SetBattleResult(GameData::BATTLE_RESULT::WIN);
	}
#endif // !_DEBUG

}