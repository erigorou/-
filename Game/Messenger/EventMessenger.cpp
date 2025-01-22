
#include "pch.h"
#include "EventMessenger.h"

// イベントリストの初期化
std::unordered_map<std::string, std::function<void()>> EventMessenger::s_eventList;


// -------------------------------------
// イベントを登録する
// -------------------------------------
void EventMessenger::Attach(const std::string& eventName, std::function<void()> function)
{
	// イベントリストにイベントを登録する
	s_eventList[eventName] = function;
}



// -------------------------------------
// イベントを実行する
// -------------------------------------
void EventMessenger::Execute(const std::string& eventName)
{
	// イベントが登録されていない場合は処理を行わない
	if (s_eventList.empty()) MessageBox(nullptr, L"イベントが登録されていません", L"エラー", MB_OK);

	// イベントリストからイベントを取り出す
	auto event = s_eventList.find(eventName);

	// イベントが見つかった場合
	if (event != s_eventList.end())
	{
		// イベントを実行する
		event->second();
	}
	else
	{
		// イベントが見つからなかった場合はエラーメッセージを表示する
		MessageBox(nullptr, L"実行しようとしたイベントは存在しません", L"エラー", MB_OK);
	}
}



// -------------------------------------
// イベントを削除する
// -------------------------------------
void EventMessenger::Detach(const std::string& eventName)
{
	// イベントリストが空の場合は処理を行わない
	if (s_eventList.empty()) MessageBox(nullptr, L"削除しようとしたイベントは存在しません", L"エラー", MB_OK);

	// イベントリストからイベントを削除する
	s_eventList.erase(eventName);
}



// -------------------------------------
// イベントリストをクリアする
// -------------------------------------
void EventMessenger::ClearEventList()
{
	// イベントリストをクリアする
	s_eventList.clear();
}