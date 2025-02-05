#include "pch.h"
#include "EventMessenger.h"

// イベントリストの初期化
std::unordered_map<EventList, std::function<void(void*)>> EventMessenger::s_eventList;
std::unordered_map<GetterList, std::function<void* ()>> EventMessenger::s_getterList;

// -------------------------------------
// イベントを登録する
// -------------------------------------
void EventMessenger::Attach(const EventList event, std::function<void(void*)> function)
{
	// イベントリストに新しいイベントを登録
	s_eventList[event] = function;
}

// -------------------------------------
// ゲッターを登録
// -------------------------------------
void EventMessenger::AttachGetter(const GetterList event, std::function<void* ()> function)
{
	// イベントリストに新しいイベントを登録
	s_getterList[event] = function;
}

// -------------------------------------
// イベントを実行する
// -------------------------------------
void EventMessenger::Execute(const EventList event, void* args)
{
	// イベントリストが空の場合
	if (s_eventList.empty())
	{
		MessageBox(nullptr, L"イベントが1つも登録されていません", L"エラー", MB_OK);
		return;
	}

	// イベントリストから指定された名前のイベントを検索
	auto foundEvent = s_eventList.find(event);

	if (foundEvent != s_eventList.end())
	{
		// void* を引数として関数を呼び出し
		foundEvent->second(args);
	}
	else
	{
		//// std::string を std::wstring に変換
		//std::wstring wideEventName(L"未登録のイベント");

		//// MessageBox に渡す
		//MessageBox(nullptr, wideEventName.c_str(), L"エラー", MB_OK);
	}
}

// -------------------------------------
// ゲッターを実行する
// -------------------------------------
void* EventMessenger::ExecuteGetter(const GetterList event)
{
	// イベントを検索
	auto foundEvent = s_getterList.find(event);

	// イベントが見つかったら実行
	if (foundEvent != s_getterList.end())
	{
		return foundEvent->second();
	}

	// イベントが見つからなかった場合、エラーメッセージを表示
	std::wstring wideEventName(L"未登録のゲッター");

	// MessageBox に渡す
	MessageBox(nullptr, wideEventName.c_str(), L"エラー", MB_OK);

	return nullptr;
}

// -------------------------------------
// イベントを削除する
// -------------------------------------
void EventMessenger::Detach(const EventList event)
{
	if (s_eventList.empty()) {
		MessageBox(nullptr, L"イベントは1つも存在しません", L"エラー", MB_OK);
		return;
	}

	s_eventList.erase(event);
}

// -------------------------------------
// イベントリストをクリアする
// -------------------------------------
void EventMessenger::ClearEventList()
{
	// 通常イベントをクリア
	s_eventList.clear();
	// ゲッターイベントをクリア
	s_getterList.clear();
}