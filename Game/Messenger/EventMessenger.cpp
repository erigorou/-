#include "pch.h"
#include "EventMessenger.h"

// イベントリストの初期化
std::unordered_map<std::string, std::function<void(void*)>> EventMessenger::s_eventList;
std::unordered_map<std::string, std::function<void*()>> EventMessenger::s_getterList;


// -------------------------------------
// イベントを登録する
// -------------------------------------
void EventMessenger::Attach(const std::string& eventName, std::function<void(void*)> function)
{
    // イベントリストに新しいイベントを登録
    s_eventList[eventName] = function;
}


// -------------------------------------
// ゲッターを登録
// -------------------------------------
void EventMessenger::AttachGetter(const std::string& eventName, std::function<void* ()> function)
{
	// イベントリストに新しいイベントを登録
	s_getterList[eventName] = function;
}


// -------------------------------------
// イベントを実行する
// -------------------------------------
void EventMessenger::Execute(const std::string& eventName, void* args)
{
    // イベントリストが空の場合
    if (s_eventList.empty()) {
        MessageBox(nullptr, L"イベントが1つも登録されていません", L"エラー", MB_OK);
        return;
    }

    // イベントリストから指定された名前のイベントを検索
    auto event = s_eventList.find(eventName);

    if (event != s_eventList.end())
    {
        // void* を引数として関数を呼び出し
        event->second(args);
    }
    else
    {
        // std::string を std::wstring に変換
        std::wstring wideEventName(eventName.begin(), eventName.end());

        // MessageBox に渡す
        MessageBox(nullptr, (L"取得しようとしたイベント '" + wideEventName + L"' は存在しません").c_str(), L"エラー", MB_OK);
    }
}


// -------------------------------------
// ゲッターを実行する
// -------------------------------------
void* EventMessenger::ExecuteGetter(const std::string& eventName)
{
    // イベントを検索
    auto event = s_getterList.find(eventName);

    // イベントが見つかったら実行
    if (event != s_getterList.end())
    {
        return event->second();
    }

    // イベントが見つからなかった場合、エラーメッセージを表示
    std::wstring wideEventName(eventName.begin(), eventName.end());
    MessageBox(nullptr, (L"取得しようとしたイベント '" + wideEventName + L"' は存在しません").c_str(), L"エラー", MB_OK);

    return nullptr;
}





// -------------------------------------
// イベントを削除する
// -------------------------------------
void EventMessenger::Detach(const std::string& eventName)
{
    if (s_eventList.empty()) {
        MessageBox(nullptr, L"イベントは1つも存在しません", L"エラー", MB_OK);
        return;
    }

    s_eventList.erase(eventName);
}


// -------------------------------------
// イベントリストをクリアする
// -------------------------------------
void EventMessenger::ClearEventList()
{
    s_eventList.clear();
}
