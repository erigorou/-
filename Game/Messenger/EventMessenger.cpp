// ----------------------------------------------------------
// 名前;	EventMessenger.cpp
// 内容:	イベントメッセンジャークラス
//			オブジェクトごとの保守性を高めるため
//			オブザーバーシステムを採用
// 作成:	池田桜輔
// ----------------------------------------------------------

// インクルード
#include "pch.h"
#include "EventMessenger.h"

// イベントリストの初期化
std::unordered_map<EventList, std::function<void(void*)>> EventMessenger::s_eventList;
std::unordered_map<GetterList, std::function<void* ()>> EventMessenger::s_getterList;

// -------------------------------------------------
/// <summary>
/// イベントを登録する
/// </summary>
/// <param name="event">イベントの種類</param>
/// <param name="function">登録する関数</param>
// -------------------------------------------------
void EventMessenger::Attach(const EventList event, std::function<void(void*)> function)
{
	// イベントリストに新しいイベントを登録
	s_eventList[event] = function;
}

// -------------------------------------------------
/// <summary>
/// ゲッターの一ベントを登録する
/// </summary>
/// <param name="event">イベントの種類</param>
/// <param name="function">登録する関数</param>
// -------------------------------------------------
void EventMessenger::AttachGetter(const GetterList event, std::function<void* ()> function)
{
	// イベントリストに新しいイベントを登録
	s_getterList[event] = function;
}

// -------------------------------------------------
/// <summary>
/// イベントを実行する
/// </summary>
/// <param name="event">イベントの種類</param>
/// <param name="args">イベントの引数</param>
// -------------------------------------------------
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
		// std::string を std::wstring に変換
		std::wstring wideEventName(L"未登録のイベント");

		// MessageBox に渡す
		MessageBox(nullptr, wideEventName.c_str(), L"エラー", MB_OK);
	}
}

// -------------------------------------------------
/// <summary>
/// ゲッターを実行する
/// </summary>
/// <param name="event">イベントの種類</param>
/// <returns>イベントの戻り値</returns>
// -------------------------------------------------
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

// -------------------------------------------------
/// <summary>
/// 登録されたイベントを解除する
/// </summary>
/// <param name="event">解除するイベントの種類</param>
// -------------------------------------------------
void EventMessenger::Detach(const EventList event)
{
	if (s_eventList.empty()) {
		MessageBox(nullptr, L"イベントは1つも存在しません", L"エラー", MB_OK);
		return;
	}

	s_eventList.erase(event);
}

// -------------------------------------------------
/// <summary>
/// イベントリストを殻にする
/// </summary>
// -------------------------------------------------
void EventMessenger::ClearEventList()
{
	// 通常イベントをクリア
	s_eventList.clear();
	// ゲッターイベントをクリア
	s_getterList.clear();
}