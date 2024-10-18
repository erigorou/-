#pragma once
#ifndef MESSENGER_DEFINED
#define MESSENGER_DEFINED

#include "Interface/IObserver.h"
#include <unordered_map>

class Messenger
{
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

private:
	// キーボード観察者リスト
	static std::vector<std::tuple<DirectX::Keyboard::Keys, IObserver*, KeyPressType>> s_observerList;
	// キー範囲リスト(開始インデックスと終了インデックス)
	static std::unordered_map<DirectX::Keyboard::Keys, std::vector<std::pair<int, int>>> s_keysRangeList;
};

#endif // MESSENGER_DEFINED

