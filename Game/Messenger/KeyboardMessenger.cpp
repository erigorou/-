// ----------------------------------------------------------
// 名前;	KeyboardMessenger.cpp
// 内容:	キーボードの入力を監視し、通知するクラス
//			オブザーバーパターンを採用
// 作成:	池田桜輔
// ----------------------------------------------------------
// インクルード
#include "pch.h"
#include "KeyboardMessenger.h"

// キーボード観察者リストを初期化する
std::vector<std::tuple<DirectX::Keyboard::Keys, IObserver*, KeyboardMessenger::KeyPressType>> KeyboardMessenger::s_observerList;
// キー範囲リスト(キー、開始インデックスと終了インデックス)
std::unordered_map<DirectX::Keyboard::Keys, std::vector<std::pair<int, int>>> KeyboardMessenger::s_keysRangeList;

// -------------------------------------------------------
/// <summary>
///	使用するキーを登録する
/// </summary>
/// <param name="key">使用するキー</param>
/// <param name="observer">通知する対象</param>
/// <param name="type">どのように押されたか</param>
// -------------------------------------------------------
void KeyboardMessenger::Attach(const DirectX::Keyboard::Keys& key, IObserver* observer, const KeyPressType type)
{
	// 観察者リストに観察者を追加する
	s_observerList.emplace_back(key, observer, type);
}

// -------------------------------------------------------
/// <summary>
/// 登録された情報を解除する
/// </summary>
/// <param name="key">解除するキー</param>
/// <param name="observer">通知していた対象</param>
/// <param name="type">どのように押されていたか</param>
// -------------------------------------------------------
void KeyboardMessenger::Detach(const DirectX::Keyboard::Keys& key, IObserver* observer, const KeyPressType type)
{
	// 観察者リストから観察者を検索する
	s_observerList.erase(
		std::remove_if(s_observerList.begin(), s_observerList.end(),
			[key, observer, type](const std::tuple<DirectX::Keyboard::Keys, IObserver*, KeyPressType>& entry)
			{
				// タプルから要素を取り出す
				auto entryKey = std::get<static_cast<int>(ArrayContentType::KEYBOARD)>(entry);
				auto entryObserver = std::get<static_cast<int>(ArrayContentType::P_OBSERVER)>(entry);
				auto entryType = std::get<static_cast<int>(ArrayContentType::TYPE)>(entry);

				// キーと観察者が一致するかどうか確認
				return entryKey == key && entryObserver == observer && entryType == type;
			}),
		s_observerList.end());
}

// -------------------------------------------------------
/// <summary>
/// キーボードの状態を通知する
/// </summary>
/// <param name="keyboardTracker">キーボードトラッカー</param>
// -------------------------------------------------------
void KeyboardMessenger::Notify(const DirectX::Keyboard::KeyboardStateTracker& keyboardTracker)
{
	auto keyboardState = keyboardTracker.GetLastState();

	// 観察者リストから観察者を取り出す
	for (const auto& keyRange : s_keysRangeList)
	{
		// 観察者が処理すべきキーかどうかを調べる
		if (keyboardTracker.IsKeyPressed(keyRange.first))
		{
			// キーの開始インデックスから終了インデックスまでのインデックスを取り出す
			for (const auto& range : keyRange.second)
			{
				for (int i = range.first; i <= range.second; ++i)
				{
					// オブザーバーの通知関数に押し下げられたキーを通知する
					auto& observer = std::get<static_cast<int>(ArrayContentType::P_OBSERVER)>(s_observerList[i]);
					auto& keyboard = std::get<static_cast<int>(ArrayContentType::KEYBOARD)>(s_observerList[i]);

					observer->OnKeyPressed(keyboard);
				}
			}
		}
	}
}

// -------------------------------------------------------
/// <summary>
/// キーボードの状態を通知する
/// </summary>
/// <param name="keyboardState">キーボードステート</param>
// -------------------------------------------------------
void KeyboardMessenger::Notify(const DirectX::Keyboard::State& keyboardState)
{
	// 観察者リストから観察者を取り出す
	for (const auto& keyRange : s_keysRangeList)
	{
		// 観察者が処理すべきキーかどうかを調べる
		if (keyboardState.IsKeyDown(keyRange.first))
		{
			// キーの開始インデックスから終了インデックスまでのインデックスを取り出す
			for (const auto& range : keyRange.second)
			{
				for (int i = range.first; i <= range.second; ++i)
				{
					// オブザーバーの通知関数に押し下げられたキーを通知する
					auto& observer = std::get<static_cast<int>(ArrayContentType::P_OBSERVER)>(s_observerList[i]);
					auto& keyboard = std::get<static_cast<int>(ArrayContentType::KEYBOARD)>(s_observerList[i]);

					observer->OnKeyDown(keyboard);
				}
			}
		}
	}
}

// -------------------------------------------------------
/// <summary>
/// 観察者リストをソートする(処理軽量化)
/// </summary>
// -------------------------------------------------------
void KeyboardMessenger::SortObserverList()
{
	// 観察者リストをキー（DirectX::Keyboard::Keys）でソートする
	std::sort(
		s_observerList.begin(),
		s_observerList.end(),
		[](const auto& a, const auto& b)
		{
			// タプルからキーを取り出す
			const auto& keyA = std::get<static_cast<int>(ArrayContentType::KEYBOARD)>(a);
			const auto& keyB = std::get<static_cast<int>(ArrayContentType::KEYBOARD)>(b);

			// キーでソートする
			return keyA < keyB;
		});
}

// -------------------------------------------------------
/// <summary>
/// キー範囲リストを生成する(処理軽量化)
/// </summary>
// -------------------------------------------------------
void KeyboardMessenger::CreateKeyRangeList()
{
	// キー範囲をクリアする
	if (!s_keysRangeList.empty()) s_keysRangeList.clear();

	// 観察者リストが空なら処理を終了する
	if (s_observerList.empty()) return;

	// 開始インデックスを設定する
	int startIndex = 0;
	DirectX::Keyboard::Keys currentKey = std::get<static_cast<int>(ArrayContentType::KEYBOARD)>(s_observerList[0]);

	// 観察者リストをループし、キー範囲を作成する
	for (int index = 1; index < s_observerList.size(); ++index)
	{
		auto key = std::get<static_cast<int>(ArrayContentType::KEYBOARD)>(s_observerList[index]);

		// 現在のキーと異なる場合、新しい範囲を追加する
		if (key != currentKey)
		{
			// キー範囲の終了インデックスを (index - 1) にする
			s_keysRangeList[currentKey].push_back(std::make_pair(startIndex, index - 1));
			// 次のキー範囲の開始インデックスを更新する
			startIndex = index;
			currentKey = key;
		}
	}

	// 最後のキー範囲を追加する
	s_keysRangeList[currentKey].push_back(std::make_pair(startIndex, static_cast<int>(s_observerList.size() - 1)));
}

// -------------------------------------------------------
/// <summary>
/// 観察者とキー範囲リストクリアする
/// </summary>
// -------------------------------------------------------
void KeyboardMessenger::Clear()
{
	// 観察者リストをクリアする
	s_observerList.clear();
	// キー範囲リストをクリアする
	s_keysRangeList.clear();
}