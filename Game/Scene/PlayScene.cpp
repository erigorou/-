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
// システム面 ================================================
#include "Game/Factory/Factory.h"						// ファクトリ
#include "Game/Sound/Sound.h"							// 音
#include "Game/HitStop/HitStop.h"						// ヒットストップ
#include "Libraries/MyLib/Collision/CollisionManager.h"	// 当たり判定
#include "Game/Data/GameData.h"							// ゲームデータ
#include"Interface/IObserver.h"							// オブザーバー
#include "Game/Messenger/KeyboardMessenger.h"			// メッセンジャー
#include "Game/Messenger/EventMessenger.h"				// イベントメッセンジャー
#include "Game/Quest/QuestManager.h"					// クエストマネージャー
// オブジェクト関連　=========================================
#include "Game/EnemyManager/EnemyManager.h"	// 敵マネージャー
#include "Game/Player/Player.h"				// プレイヤー
#include "Game/Boss/Boss.h"					// 鬼
#include "Game/Weapon/Sword/Sword.h"		// 刀
#include "Game/Weapon/Cudgel/Cudgel.h"		// 金棒
#include "Game/Goblin/Goblin.h"				// ゴブリン

#include "Game/Stage/Floor/Floor.h"			// 床
#include "Game/Stage/Sea/Sea.h"				// 海
#include "Game/Stage/Wall/Wall.h"			// 壁
// UI関連　====================================================
#include "Game/UI/!PlaySceneUIManager/PlaySceneUIManager.h"	// UI描画関連


// ----------------
// コンストラクタ
// ----------------
PlayScene::PlayScene()
	: m_commonResources{}
	, m_debugCamera{}
	, m_projection{}
	, m_isChangeScene{false}
	, m_smoothDeltaTime{}
{
	m_commonResources = CommonResources::GetInstance();
	GameData::GetInstance()->SetBattleResult(GameData::BATTLE_RESULT::NONE);
}


// ----------------
// デストラクタ
// ----------------
PlayScene::~PlayScene()
{
}


// ----------------
// 初期化関数
// ----------------
void PlayScene::Initialize()
{
	// デバッグカメラを作成する
	RECT rect{ m_commonResources->GetDeviceResources()->GetOutputSize() };

	m_debugCamera = std::make_unique<mylib::DebugCamera>();
	m_debugCamera->Initialize(rect.right, rect.bottom);

	// 射影行列を作成する
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		DirectX::XMConvertToRadians(FOV),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		NEAR_Z, FAR_Z);

	// オブジェクトの生成
	CreateObjects();
}


// ----------------
// オブジェクトの生成
// ----------------
void PlayScene::CreateObjects()
{
	KeyboardMessenger::Clear();	// メッセンジャーのクリア

	m_hitStop = HitStop::GetInstance();

	m_collisionManager	= Factory::CreateCollisionManager	();	// パーティクル
	m_camera			= Factory::CreateCamera				();	// カメラ
	m_particles			= Factory::CreateParticle			();	// パーティクル

	m_skySphere			= Factory::CreateSkySphere			();	// 天球	
	m_floor				= Factory::CreateFloor				();	// フロア
	m_sea				= Factory::CreateSea				();	// 海
	m_wall				= Factory::CreateWall				();	// 壁
	m_player			= Factory::CreatePlayer				(this);	// プレイヤ

	m_uiManager		= Factory::CreateUIManager				(this);	// UIマネージャー
	m_enemyManager	= Factory::CreateEnemyManager			(this);	// 敵マネージャー
	m_questManager	= Factory::CreateQuestManager			(this);	// クエストマネージャー


	m_uiManager->CreateUI();	// UIの生成
	// 観察者リストをソートする
	KeyboardMessenger::SortObserverList();
	// キー範囲リストを生成する
	KeyboardMessenger::CreateKeyRangeList();
	// カメラをプレイシーンで設定
	m_camera->ChangeState(m_camera->GetPlayState());
	// BGM変更
	Sound::GetInstance()->ChangeBGM(Sound::BGM_TYPE::PLAY);
}



// --------------------------------
// キーが押されたかどうかを判定する
// --------------------------------
inline bool IsKeyPress(DirectX::Keyboard::KeyboardStateTracker& stateTracker)
{
	// すべてのキーが押されたかどうかをチェック
	for (int key = 0; key < PlayScene::MAX_KEY; key++)
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


// --------------------------------
// キーが押下げられたかどうかを判定する
// --------------------------------
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


// --------------------------------
// 更新処理
// --------------------------------
void PlayScene::Update(float elapsedTime)
{
	// キーボードの更新処理
	UpdateKeyboard();
	// オブジェクトの更新処理
	UpdateObjects(elapsedTime);
}


// --------------------------------
// キーボードの更新処理
// --------------------------------
void PlayScene::UpdateKeyboard()
{
	// キーボードの状態を取得する
	m_keyboardState = DirectX::Keyboard::Get().GetState();
	// キーボードステートトラッカーを更新する
	m_keyboardStateTracker.Update(m_keyboardState);

	// キーボードが押下げられたかどうかを判定する
	if (IsKeyDown(m_keyboardState))			KeyboardMessenger::Notify(m_keyboardState);
	if (IsKeyPress(m_keyboardStateTracker))	KeyboardMessenger::Notify(m_keyboardStateTracker);

	if (m_keyboardStateTracker.IsKeyPressed(DirectX::Keyboard::Keys::Space)) m_enemyManager->ChangeCameraTarget();
}


// --------------------------------
// オブジェクトの更新処理
// --------------------------------
void PlayScene::UpdateObjects(float elapsedTime)
{
	// ヒットストップの更新
	m_hitStop->Update(elapsedTime);
	// ヒットストップの残り時間を取得
	float smoothDeltaTime = m_hitStop->GetSmoothDeltaTime();

	// オブジェクトの更新
	m_uiManager		->Update(elapsedTime);
	m_player		->Update(smoothDeltaTime);
	m_enemyManager	->Update(smoothDeltaTime);
	m_questManager	->Update(elapsedTime);
	UpdateCamera(elapsedTime);

	// パーティクルの更新
	m_particles->Update(elapsedTime,m_player->GetPosition(),m_player->GetVelocity());

	// 衝突判定の更新処理
	m_collisionManager->Update();


	GameOverChacker();	// ゲームオーバー判定
}


// --------------------------------
// 描画関数
// --------------------------------
void PlayScene::Render()
{
	// ビュー行列を取得する
	const DirectX::SimpleMath::Matrix& view = m_camera->GetViewMatrix();

	m_collisionManager->Render(view, m_projection);	// 当たり判定

	m_skySphere->	DrawSkySphere	(view, m_projection);	// 空
	m_floor->		Render			(view, m_projection);	// 地面
	m_sea->			Render			(view, m_projection);	// 海
	m_wall->		Render			(view, m_projection);	// 壁

	m_player->		Render(view, m_projection);	// プレイヤー
	m_enemyManager->Render(view, m_projection); // 敵（複数）
	
	DrawParticle(view, m_projection);	// パーティクル

	m_questManager->DrawQuest();	// クエスト
	m_uiManager->Render();			// UI
}

// --------------------------------
// パーティクルの描画
// --------------------------------
void PlayScene::DrawParticle(const DirectX::SimpleMath::Matrix& view, DirectX::SimpleMath::Matrix projection)
{
	// ビルボード行列の計算
	m_particles->CreateBillboard(m_camera->GetTargetPosition(), m_camera->GetEyePosition(), DirectX::SimpleMath::Vector3::Up);
	// パーティクルの描画
	m_particles->Render(view, projection);
}


// --------------------------------
// 終了関数
// --------------------------------
void PlayScene::Finalize()
{
}

// --------------------------------
// カメラの更新
// --------------------------------
void PlayScene::UpdateCamera(float elapsedTime)
{
	// カメラの更新
	m_camera->Update(m_player->GetPosition(), m_enemyManager->GetPicupEnemyPosition(), elapsedTime);
}

// --------------------------------
// ボスのポインタを取得
// --------------------------------
Boss* PlayScene::GetBoss()
{
	return m_enemyManager->GetBossEnemy();
}

// --------------------------------
// ターゲットとなる敵の座標を取得
// --------------------------------
DirectX::SimpleMath::Vector3 PlayScene::GetTargetPosition()
{
	return m_enemyManager->GetPicupEnemyPosition();
}


// --------------------------------
// 次のシーンIDを取得
// --------------------------------
IScene::SceneID PlayScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		return IScene::SceneID::QUEST;
	}

	// シーン変更がない場合
	return IScene::SceneID::NONE;
}



// --------------------------------
// リザルトに行けるかどうかを判定
// --------------------------------
void PlayScene::CheckResult()
{
	m_isChangeScene = true;
}


// --------------------------------
// ゲームオーバー判定
// --------------------------------
void PlayScene::GameOverChacker()
{
	static bool flag = true;

	if (m_keyboardStateTracker.IsKeyPressed(DirectX::Keyboard::Keys::F2)) {
		flag = !flag;
	}

	if (flag) return;

	// プレイヤーが死亡
	if (m_player->GetPlayerHP()->GetHP() <= 0)
	{
		GameEnd();
	}
}


// --------------------------------
// ゲーム終了処理
// --------------------------------
void PlayScene::GameEnd()
{
	CheckResult();
}
