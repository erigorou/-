#pragma once
#ifndef MESSENGER_H
#define MESSENGER_H

#include "pch.h"
#include <unordered_map>
#include <functional>
#include <string>

class EventMessenger
{
public:
    // イベントを登録する
    static void Attach(const std::string& eventName, std::function<void(void*)> function);

    // イベントを実行する
    static void Execute(const std::string& eventName, void* args);

    // イベントを削除する
    static void Detach(const std::string& eventName);

    // イベントリストをクリアする
    static void ClearEventList();

private:
    // 登録されたイベントを保存する変数
    static std::unordered_map<std::string, std::function<void(void*)>> s_eventList;
};

#endif // !MESSENGER_H
