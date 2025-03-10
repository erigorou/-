// ---------------------------------------------
// 
// 名前:	SceneManager.cpp
// 内容:	シーンマネージャクラス
// 作成:	池田桜輔
// 
// ---------------------------------------------
// インクルード
#include "pch.h"
#include "SceneManager.h"
#include "Game/UI/Fade/Fade.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "ResultScene.h"
#include "Game/Data/GameData.h"
#include "Game/GameResources.h"
#include "../Sound/Sound.h"

#include "Keyboard.h"

//---------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
//---------------------------------------------------------
SceneManager::SceneManager()
	: 
	m_currentScene{},
	m_nextSceneID(IScene::SceneID::NONE),
	m_currentNextSceneID(IScene::SceneID::TITLE),
	m_canChangeScene(false),
	m_isFade(false)
{
	m_gameResources = GameResources::GetInstance();
	m_sound = Sound::GetInstance();
}


//---------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
//---------------------------------------------------------
SceneManager::~SceneManager()
{
	Finalize();
}

//---------------------------------------------------------
/// <summary>
/// 初期化する
/// </summary>
//---------------------------------------------------------
void SceneManager::Initialize()
{
	// フェードを作成
	m_fade = std::make_unique<Fade>(this);
	m_fade->Initialize();

// デバッグならこれ
#ifdef _DEBUG
	ChangeScene(IScene::SceneID::TITLE);
#endif // !_DEBUG

// リリースならこれ
#ifndef _DEBUG
	ChangeScene(IScene::SceneID::TITLE);
#endif // !_DEBUG
}

//---------------------------------------------------------
/// <summary>
/// 更新する
/// </summary>
//---------------------------------------------------------
void SceneManager::Update(float elapsedTime)
{
	// Escapeキーの入力検知
	CheckEscapeKey();
	// 秒数の保存
	GameData::GetInstance()->SetElapsedTime(elapsedTime);
	// 効果音の再生
	Sound::Update();
	// 各シーンの更新
	m_currentScene->Update(elapsedTime);
	// フェードの更新
	m_fade->Update(elapsedTime);

	// 説明用変数：次のシーン
	m_currentNextSceneID = m_currentScene->GetNextSceneID();

	// シーンを変更しないとき
	if (m_currentNextSceneID == IScene::SceneID::NONE && !m_isFade) return;

	// 初回セットアップ
	if (m_nextSceneID == IScene::SceneID::NONE)
	{
		m_nextSceneID = m_currentNextSceneID;
		m_isFade = true;
		m_fade->StartFadeOut();
		m_canChangeScene = false;
	}

	// シーン変更フラグをFadeクラスからもらったらtrueにする
	if (m_canChangeScene)
	{
		// フェードを終了
		m_isFade = false;
		// シーンを変更
		ChangeScene(m_nextSceneID);
		// シーンIDを初期化
		m_nextSceneID = IScene::SceneID::NONE;
		// シーン変更を禁止
		m_canChangeScene = false;
	}
}

//---------------------------------------------------------
/// <summary>
/// 描画する
/// </summary>
//---------------------------------------------------------
void SceneManager::Render()
{
	// シーンの描画
	m_currentScene->Render();
	// フェードの描画
	m_fade->Render();
}

//---------------------------------------------------------
/// <summary>
/// 後始末する
/// </summary>
//---------------------------------------------------------
void SceneManager::Finalize()
{
	// シーンを削除
	DeleteScene();
}

//---------------------------------------------------------
/// <summary>
/// シーンを変更する
/// </summary>
//---------------------------------------------------------
void SceneManager::ChangeScene(IScene::SceneID sceneID)
{
	// シーンを削除
	DeleteScene();
	// シーンを作成
	CreateScene(sceneID);
	// フェードイン
	m_fade->StartFadeIn();
}

//---------------------------------------------------------
/// <summary>
/// シーンを作成する
/// </summary>
//---------------------------------------------------------
void SceneManager::CreateScene(IScene::SceneID sceneID)
{
	assert(m_currentScene == nullptr);

	switch (sceneID)
	{
	case IScene::SceneID::TITLE:
		m_currentScene = std::make_unique<TitleScene>();
		break;

	case IScene::SceneID::PLAY:
		m_currentScene = std::make_unique<PlayScene>();
		break;

	case IScene::SceneID::RESULT:
		m_currentScene = std::make_unique<ResultScene>();
		break;

	default:
		assert(!"SceneManager::CreateScene::シーン名が存在しません！");
		// no break
	}

	m_currentScene->Initialize();
}

//---------------------------------------------------------
/// <summary>
/// シーンを削除する
/// </summary>
//---------------------------------------------------------
void SceneManager::DeleteScene()
{
	if (m_currentScene)
	{
		m_currentScene.reset();
	}
}

//---------------------------------------------------------
/// <summary>
/// Escapeキーの入力検知
/// </summary>
// ---------------------------------------------------------
void SceneManager::CheckEscapeKey()
{
	m_keyboardState = DirectX::Keyboard::Get().GetState();
	m_keyboardTracker.Update(m_keyboardState);

	// ESCAPEキーが押されたら
	if (m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::Escape))
	{
		// 各シーンでのESCAPEキーの挙動
		EscapeAction();
	}
}

// ---------------------------------------------------------
/// <summary>
/// 各シーンでのESCAPEキーの挙動
/// </summary>
// ---------------------------------------------------------
void SceneManager::EscapeAction()
{
	// 現在がタイトルシーンなら
	if (m_currentScene->GetCurrentSceneID() == IScene::SceneID::TITLE)
	{
		// ゲームを終了する
		PostQuitMessage(0);
	}

	// 次のシーンをタイトルに変更する
	m_nextSceneID = IScene::SceneID::TITLE;
	// フェードアウト開始
	m_canChangeScene = true;
}
