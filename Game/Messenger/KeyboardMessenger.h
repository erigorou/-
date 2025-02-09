// ----------------------------------------------------------
// 名前;	KeyboardMessenger.cpp
// 内容:	キーボードの入力を監視し、通知するクラス
//			オブザーバーパターンを採用
// 作成:	池田桜輔
// ----------------------------------------------------------
#pragma once
#ifndef MESSENGER_DEFINED
#define MESSENGER_DEFINED

// インクルード
#include "Interface/IObserver.h"
#include <unordered_map>

/// <summary>
/// キー入力を監視し通知する
/// </summary>
class KeyboardMessenger
{
	// ------------------
	// 列挙型
	// ------------------
public:
	// 通知の仕方
	enum class KeyPressType : UINT
	{
		DOWN,
		PRESSED
	};

	// 配列の内容
	enum class ArrayContentType : int
	{
		KEYBOARD,
		P_OBSERVER,
		TYPE
	};

	// ------------------
	// メンバ関数
	// ------------------
public:
	// 観察者をアタッチする
	static void Attach(const DirectX::Keyboard::Keys& key, IObserver* observer, const KeyPressType type);
	// 観察者をデタッチする
	static void Detach(const DirectX::Keyboard::Keys& key, IObserver* observer, const KeyPressType type);
	// 通知する
	static void Notify(const DirectX::Keyboard::State& keyboardState);
	static void Notify(const DirectX::Keyboard::KeyboardStateTracker& keyboardTracker);
	// 観察者リストをソートする
	static void SortObserverList();
	// キー範囲リストを生成する
	static void CreateKeyRangeList();
	// 観察者リストとキー範囲リストをクリアする
	static void Clear();

	// ------------------
	// メンバ変数
	// ------------------
private:
	// キーボード観察者リスト
	static std::vector<std::tuple<DirectX::Keyboard::Keys, IObserver*, KeyPressType>> s_observerList;
	// キー範囲リスト(開始インデックスと終了インデックス)
	static std::unordered_map<DirectX::Keyboard::Keys, std::vector<std::pair<int, int>>> s_keysRangeList;
};

#endif // MESSENGER_DEFINED
