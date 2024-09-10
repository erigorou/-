// -----------------------------------------------
//
// プレイ中のUIの描画や生成を行う関数
//
// -----------------------------------------------

#pragma once

#ifndef PLAYSCENE_UI_MANAGER
#define PLAYSCENE_UI_MANAGER

#include "pch.h"
#include <unordered_map>

// 前方宣言
class IUserInterface;

class PlaySceneUIManager
{
public:

	// コンストラクタ
	PlaySceneUIManager();
	// デストラクタ
	~PlaySceneUIManager();
	// 初期化関数
	void Initialize();
	// 更新関数
	void Update();
	// 描画関数
	void Render();
	// 終了関数
	void Finalize();

	// 追加関数
	void Add(IUserInterface* ui, std::string key);
	// 削除関数
	void Remove(std::string key);

private:
	// UIのポインタを格納するリスト
	std::unordered_map<std::string, IUserInterface*> m_uiList;
};

#endif