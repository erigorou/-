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
#include "Game/Messenger/KeyboardMessenger.h"

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
	// キーボード使用検知
	UpdateKeyboard();

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

	// 観察者リストを初期化する
	KeyboardMessenger::Clear();
	// シーンを初期化
	m_currentScene->Initialize();
	// 観察者リストをソートする
	KeyboardMessenger::SortObserverList();
	// キー範囲リストを生成する
	KeyboardMessenger::CreateKeyRangeList();
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


// ------------------------------------------------------------------------------
/// <summary>
/// キーが押されたかどうかを判定する
/// </summary>
/// <param name="stateTracker">キーボードトラッカー</param>
/// <returns>押されたかのフラグ</returns>
// ------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------
/// <summary>
/// キーが押されているかどうかを判定する
/// </summary>
/// <param name="state">キーボードステート</param>
/// <returns>押されているかのフラグ</returns>
// ------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------
/// <summary>
/// キーボードの入力を更新する
/// </summary>
// ------------------------------------------------------------------------------
void SceneManager::UpdateKeyboard()
{
	// キーボードの状態を取得する
	m_keyboardState = DirectX::Keyboard::Get().GetState();
	// キーボードステートトラッカーを更新する
	m_keyboardStateTracker.Update(m_keyboardState);

	// キーボードが押下げられたかどうかを判定する
	if (IsKeyDown(m_keyboardState))			KeyboardMessenger::Notify(m_keyboardState);
	if (IsKeyPress(m_keyboardStateTracker))	KeyboardMessenger::Notify(m_keyboardStateTracker);
}