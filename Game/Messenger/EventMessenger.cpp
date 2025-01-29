#include "pch.h"
#include "EventMessenger.h"

// イベントリストの初期化
std::unordered_map<std::string, std::function<void(void*)>> EventMessenger::s_eventList;


// -------------------------------------
// イベントを登録する
// -------------------------------------
void EventMessenger::Attach(const std::string& eventName, std::function<void(void*)> function)
{
    // イベントリストに新しいイベントを登録
    s_eventList[eventName] = function;
}


// -------------------------------------
// イベントを実行する
// -------------------------------------
void EventMessenger::Execute(const std::string& eventName, void* args)
{
    // イベントリストが空の場合
    if (s_eventList.empty()) {
        MessageBox(nullptr, L"イベントが登録されていません", L"エラー", MB_OK);
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
        MessageBox(nullptr, L"実行しようとしたイベントは存在しません", wideEventName.c_str(), MB_OK);
    }
}


// -------------------------------------
// イベントを削除する
// -------------------------------------
void EventMessenger::Detach(const std::string& eventName)
{
    if (s_eventList.empty()) {
        MessageBox(nullptr, L"削除しようとしたイベントは存在しません", L"エラー", MB_OK);
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
