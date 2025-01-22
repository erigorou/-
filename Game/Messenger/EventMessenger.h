#pragma once
#ifndef MESSENGER_H
#define MESSENGER_H
#include "pch.h"
#include <unordered_map>


class EventMessenger
{
// -------------------------------------
// ���J�֐�
// -------------------------------------
public:

	// �C�x���g��o�^����
	static void Attach(const std::string& eventName, std::function<void()> function);

	// �C�x���g�����s����
	static void Execute(const std::string& eventName);

	// �C�x���g���폜����
	static void Detach(const std::string& eventName);

	// �C�x���g���X�g���N���A����
	static void ClearEventList();



// -------------------------------------
// �����o�ϐ�
// -------------------------------------
private:

	// �o�^���ꂽ�C�x���g��ۑ�����ϐ�
	static std::unordered_map<std::string, std::function<void()>> s_eventList;
};

#endif // !MESSENGER_H