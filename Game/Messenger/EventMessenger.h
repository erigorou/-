#pragma once
#ifndef EVENT_MESSENGER_H
#define EVENT_MESSENGER_H

#include "pch.h"
#include <unordered_map>
#include <functional>
#include <string>

class EventMessenger
{
public:
    // �C�x���g��o�^����
    static void Attach(const std::string& eventName, std::function<void(void*)> function);

    // �Q�b�^�[��o�^
    static void AttachGetter(const std::string& eventName, std::function<void* ()> function);

    // �C�x���g�����s����
    static void Execute(const std::string& eventName, void* args);

	// �Q�b�^�[�����s����
	static void* ExecuteGetter(const std::string& eventName);

    // �C�x���g���폜����
    static void Detach(const std::string& eventName);

    // �C�x���g���X�g���N���A����
    static void ClearEventList();

private:
    // �o�^���ꂽ�C�x���g��ۑ�����ϐ�
    static std::unordered_map<std::string, std::function<void(void*)>> s_eventList;
	static std::unordered_map<std::string, std::function<void*()>> s_getterList;
};

#endif // !MESSENGER_H
