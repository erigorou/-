
#include "pch.h"
#include "EventMessenger.h"

// �C�x���g���X�g�̏�����
std::unordered_map<std::string, std::function<void()>> EventMessenger::s_eventList;


// -------------------------------------
// �C�x���g��o�^����
// -------------------------------------
void EventMessenger::Attach(const std::string& eventName, std::function<void()> function)
{
	// �C�x���g���X�g�ɃC�x���g��o�^����
	s_eventList[eventName] = function;
}



// -------------------------------------
// �C�x���g�����s����
// -------------------------------------
void EventMessenger::Execute(const std::string& eventName)
{
	// �C�x���g���o�^����Ă��Ȃ��ꍇ�͏������s��Ȃ�
	if (s_eventList.empty()) MessageBox(nullptr, L"�C�x���g���o�^����Ă��܂���", L"�G���[", MB_OK);

	// �C�x���g���X�g����C�x���g�����o��
	auto event = s_eventList.find(eventName);

	// �C�x���g�����������ꍇ
	if (event != s_eventList.end())
	{
		// �C�x���g�����s����
		event->second();
	}
	else
	{
		// �C�x���g��������Ȃ������ꍇ�̓G���[���b�Z�[�W��\������
		MessageBox(nullptr, L"���s���悤�Ƃ����C�x���g�͑��݂��܂���", L"�G���[", MB_OK);
	}
}



// -------------------------------------
// �C�x���g���폜����
// -------------------------------------
void EventMessenger::Detach(const std::string& eventName)
{
	// �C�x���g���X�g����̏ꍇ�͏������s��Ȃ�
	if (s_eventList.empty()) MessageBox(nullptr, L"�폜���悤�Ƃ����C�x���g�͑��݂��܂���", L"�G���[", MB_OK);

	// �C�x���g���X�g����C�x���g���폜����
	s_eventList.erase(eventName);
}



// -------------------------------------
// �C�x���g���X�g���N���A����
// -------------------------------------
void EventMessenger::ClearEventList()
{
	// �C�x���g���X�g���N���A����
	s_eventList.clear();
}