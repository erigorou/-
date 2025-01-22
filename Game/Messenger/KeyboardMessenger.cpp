

#include "pch.h"
#include "KeyboardMessenger.h"

// キーボード観察者リストを初期化する
std::vector<std::tuple<DirectX::Keyboard::Keys, IObserver*, KeyboardMessenger::KeyPressType>> KeyboardMessenger::s_observerList;
// キー範囲リスト(キー、開始インデックスと終了インデックス)
std::unordered_map<DirectX::Keyboard::Keys, std::vector<std::pair<int, int>>> KeyboardMessenger::s_keysRangeList;


// 観察者をアタッチする
void KeyboardMessenger::Attach(const DirectX::Keyboard::Keys& key, IObserver* observer, const KeyPressType type)
{
    // 観察者リストに観察者を追加する
    s_observerList.emplace_back(key, observer, type);
}


// 観察者をデタッチする
void KeyboardMessenger::Detach(const DirectX::Keyboard::Keys& key, IObserver* observer, const KeyPressType type)
{
    // 観察者リストから観察者を検索する
    s_observerList.erase(
        std::remove_if(s_observerList.begin(), s_observerList.end(),
            [key, observer, type](const std::tuple<DirectX::Keyboard::Keys, IObserver*, KeyPressType>& entry)
            {
                // タプルから要素を取り出す
                auto entryKey       = std::get<static_cast<int>(ArrayContentType::KEYBOARD  )>(entry);
                auto entryObserver  = std::get<static_cast<int>(ArrayContentType::P_OBSERVER)>(entry);
                auto entryType      = std::get<static_cast<int>(ArrayContentType::TYPE      )>(entry);

                // キーと観察者が一致するかどうか確認
                return entryKey == key && entryObserver == observer && entryType == type;
            }),
        s_observerList.end());
}



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


// 観察者リストをソートする
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


// キー範囲リストを生成する
void KeyboardMessenger::CreateKeyRangeList()
{
    // キー範囲をクリアする
	if (s_keysRangeList.size()) s_keysRangeList.clear();
    // 開始インデックスを設定する
	int startIndex = 0;
    // 観察者リストの開始キーを現在のキーにする
    DirectX::Keyboard::Keys currentKey = std::get<static_cast<int>(ArrayContentType::KEYBOARD)>(s_observerList[startIndex]);

    for (int index = 1; index < s_observerList.size(); index++)
    {
        // 観察者リストの現在キーと現在のキーが等しくない場合
		auto key = std::get<static_cast<int>(ArrayContentType::KEYBOARD)>(s_observerList[index]);
        if (key != currentKey)
        {
            // キー、開始インデックスと終了インデックスをキー範囲リストに追加する
            s_keysRangeList[currentKey].push_back(std::make_pair(startIndex, index - 1));
            // 現在のインデックスを開始インデックスに設定する
            startIndex = index;
            // 現在の観察者のキーを現在のキーに設定する
			currentKey = key;
        }
    }
    // キー、開始インデックスと最後のインデックスをキー範囲リストに追加する
    s_keysRangeList[currentKey].push_back(std::make_pair(startIndex, s_observerList.size() - 1));
}


// 観察者リストとキー範囲リストをクリアする
void KeyboardMessenger::Clear()
{
	// 観察者リストをクリアする
	s_observerList.clear();
	// キー範囲リストをクリアする
	s_keysRangeList.clear();
}
