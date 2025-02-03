#pragma once
#ifndef EVENT_MESSENGER_H
#define EVENT_MESSENGER_H

#include "pch.h"
#include <unordered_map>
#include <functional>
#include <string>
#include "EventList.h"

class EventMessenger
{
public:
    // イベントを登録する
    static void Attach(const EventList event, std::function<void(void*)> function);
    // ゲッターを登録
    static void AttachGetter(const GetterList event, std::function<void*()> function);
    // イベントを実行する
    static void Execute(const EventList event, void* args);
	// ゲッターを実行する
	static void* ExecuteGetter(const GetterList event);
    // イベントを削除する
    static void Detach(const EventList event);
    // イベントリストをクリアする
    static void ClearEventList();
private:
    // 登録されたイベントを保存する変数
    static std::unordered_map<EventList, std::function<void(void*)>> s_eventList;
	static std::unordered_map<GetterList, std::function<void*()>> s_getterList;
};

#endif // !MESSENGER_H
