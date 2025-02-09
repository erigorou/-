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
#include <cassert>

//---------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
//---------------------------------------------------------
SceneManager::SceneManager()
	: 
	m_currentScene{},
	m_nextSceneID(IScene::SceneID::NONE),
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
	m_fade = std::make_unique<Fade>(this);
	m_fade->Initialize();

#ifndef _DEBUG
	ChangeScene(IScene::SceneID::TITLE);
#endif // !_DEBUG
#ifdef _DEBUG
	ChangeScene(IScene::SceneID::RESULT);
#endif // !_DEBUG
}

//---------------------------------------------------------
/// <summary>
/// 更新する
/// </summary>
//---------------------------------------------------------
void SceneManager::Update(float elapsedTime)
{
	// 秒数の保存
	GameData::GetInstance()->SetElapsedTime(elapsedTime);
	// 効果音の再生
	Sound::Update();
	// 各シーンの更新
	m_currentScene->Update(elapsedTime);
	// フェードの更新
	m_fade->Update(elapsedTime);

	// 説明用変数：次のシーン
	const IScene::SceneID nextSceneID = m_currentScene->GetNextSceneID();

	// シーンを変更しないとき
	if (nextSceneID == IScene::SceneID::NONE && !m_isFade) return;

	// 初回セットアップ
	if (m_nextSceneID == IScene::SceneID::NONE)
	{
		m_nextSceneID = nextSceneID;
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
