/*
	@file	SceneManager.cpp
	@brief	シーンマネージャクラス
*/
#include "pch.h"
#include "SceneManager.h"
#include "Game/UI/Fade/Fade.h"

#include "TitleScene.h"
#include "PlayScene.h"
#include "QuestSelectScene.h"
#include "ResultScene.h"

#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include "../Sound/Sound.h"
#include <cassert>


//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
SceneManager::SceneManager()
	: m_currentScene{}
	, m_commonResources{}
	, m_nextSceneID(IScene::SceneID::NONE)
	, m_canChangeScene(false)
	, m_isFade(false)
{
	m_commonResources	= CommonResources::	GetInstance();
	m_sound				= Sound::			GetInstance();
}

//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
SceneManager::~SceneManager()
{
	Finalize();
}

//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void SceneManager::Initialize()
{
	m_fade = std::make_unique<Fade>(this);
	m_fade->Initialize();

#ifndef _DEBUG
	ChangeScene(IScene::SceneID::TITLE);
#endif // !_DEBUG
#ifdef _DEBUG
	ChangeScene(IScene::SceneID::QUEST);
#endif // !_DEBUG

}

//---------------------------------------------------------
// 更新する
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
	if (nextSceneID == IScene::SceneID::NONE && ! m_isFade) return;

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
		m_isFade = false;						// フェードを終了
		ChangeScene(m_nextSceneID);				// シーンを変更
		m_nextSceneID = IScene::SceneID::NONE;	// シーンIDを初期化
		m_canChangeScene = false;				// シーン変更を禁止
	}
}

//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void SceneManager::Render()
{
	m_currentScene->Render();

	m_fade->Render();
}

//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void SceneManager::Finalize()
{
	DeleteScene();
}

//---------------------------------------------------------
// シーンを変更する
//---------------------------------------------------------
void SceneManager::ChangeScene(IScene::SceneID sceneID)
{
	DeleteScene();
	CreateScene(sceneID);

	m_fade->StartFadeIn();
}

//---------------------------------------------------------
// シーンを作成する
//---------------------------------------------------------
void SceneManager::CreateScene(IScene::SceneID sceneID)
{
	assert(m_currentScene == nullptr);

	switch (sceneID)
	{
		case IScene::SceneID::TITLE:
			m_currentScene = std::make_unique<TitleScene>();
			break;

		case IScene::SceneID::QUEST:
			m_currentScene = std::make_unique<QuestSelectScene>();
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

	assert(m_currentScene && "SceneManager::CreateScene::次のシーンが生成されませんでした！");
	m_currentScene->Initialize();
}

//---------------------------------------------------------
// シーンを削除する
//---------------------------------------------------------
void SceneManager::DeleteScene()
{
	if (m_currentScene)
	{
		m_currentScene.reset();
	}
}
