// -----------------------------------------------
//
// プレイ中のUIの描画や生成を行う関数
//
// -----------------------------------------------

#include "pch.h"
#include "Game/UI/PlaySceneUIManager/Header/PlaySceneUIManager.h"

#include "Interface/IUserInterface.h"



// ----------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// ----------------------------
PlaySceneUIManager::PlaySceneUIManager()
{
	// 配列内をリセット
	m_uiList.clear();
}



// ----------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ----------------------------
PlaySceneUIManager::~PlaySceneUIManager()
{
}



// ----------------------------
/// <summary>
/// 初期化関数
/// </summary>
// ----------------------------
void PlaySceneUIManager::Initialize()
{
}


// ----------------------------
/// <summary>
/// 更新関数
/// </summary>
// ----------------------------
void PlaySceneUIManager::Update()
{
	// その他のUIの全ての更新処理を行う
	for (const auto& pair : m_uiList)
	{
		pair.second->Update();
	}
}



// ----------------------------
/// <summary>
/// 描画関数
/// </summary>
// ----------------------------
void PlaySceneUIManager::Render()
{
	// その他のUIの全ての描画処理を行う
	for (const auto& pair : m_uiList)
	{
		pair.second->Render();
	}
}



// ----------------------------
/// <summary>
/// 終了関数
/// </summary>
// ----------------------------
void PlaySceneUIManager::Finalize()
{
	// その他のUIの全ての終了処理を行う
	for (const auto& pair : m_uiList)
	{
		pair.second->Finalize();
	}
}



// ----------------------------------------------------------
/// <summary>
/// 追加関数
/// </summary>
/// <param name="ui">追加したいUI　IUserInterface</param>
// ----------------------------------------------------------
void PlaySceneUIManager::Add(IUserInterface* ui, std::string key)
{
	// uiのリストに追加をする
	m_uiList[key] = ui;
}



// ----------------------------
/// <summary>
/// 削除関数
/// </summary>
/// <param name="key">キー</param>
// ----------------------------
void PlaySceneUIManager::Remove(std::string key)
{
	m_uiList[key]->	Finalize();	// 終了処理
	m_uiList.		erase(key);	// 削除
}
