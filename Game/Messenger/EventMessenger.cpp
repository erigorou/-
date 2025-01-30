#include "pch.h"
#include "EventMessenger.h"

// �C�x���g���X�g�̏�����
std::unordered_map<std::string, std::function<void(void*)>> EventMessenger::s_eventList;
std::unordered_map<std::string, std::function<void*()>> EventMessenger::s_getterList;


// -------------------------------------
// �C�x���g��o�^����
// -------------------------------------
void EventMessenger::Attach(const std::string& eventName, std::function<void(void*)> function)
{
    // �C�x���g���X�g�ɐV�����C�x���g��o�^
    s_eventList[eventName] = function;
}


// -------------------------------------
// �Q�b�^�[��o�^
// -------------------------------------
void EventMessenger::AttachGetter(const std::string& eventName, std::function<void* ()> function)
{
	// �C�x���g���X�g�ɐV�����C�x���g��o�^
	s_getterList[eventName] = function;
}


// -------------------------------------
// �C�x���g�����s����
// -------------------------------------
void EventMessenger::Execute(const std::string& eventName, void* args)
{
    // �C�x���g���X�g����̏ꍇ
    if (s_eventList.empty()) {
        MessageBox(nullptr, L"�C�x���g��1���o�^����Ă��܂���", L"�G���[", MB_OK);
        return;
    }

    // �C�x���g���X�g����w�肳�ꂽ���O�̃C�x���g������
    auto event = s_eventList.find(eventName);

    if (event != s_eventList.end())
    {
        // void* �������Ƃ��Ċ֐����Ăяo��
        event->second(args);
    }
    else
    {
        // std::string �� std::wstring �ɕϊ�
        std::wstring wideEventName(eventName.begin(), eventName.end());

        // MessageBox �ɓn��
        MessageBox(nullptr, (L"�擾���悤�Ƃ����C�x���g '" + wideEventName + L"' �͑��݂��܂���").c_str(), L"�G���[", MB_OK);
    }
}


// -------------------------------------
// �Q�b�^�[�����s����
// -------------------------------------
void* EventMessenger::ExecuteGetter(const std::string& eventName)
{
    // �C�x���g������
    auto event = s_getterList.find(eventName);

    // �C�x���g��������������s
    if (event != s_getterList.end())
    {
        return event->second();
    }

    // �C�x���g��������Ȃ������ꍇ�A�G���[���b�Z�[�W��\��
    std::wstring wideEventName(eventName.begin(), eventName.end());
    MessageBox(nullptr, (L"�擾���悤�Ƃ����C�x���g '" + wideEventName + L"' �͑��݂��܂���").c_str(), L"�G���[", MB_OK);

    return nullptr;
}





// -------------------------------------
// �C�x���g���폜����
// -------------------------------------
void EventMessenger::Detach(const std::string& eventName)
{
    if (s_eventList.empty()) {
        MessageBox(nullptr, L"�C�x���g��1�����݂��܂���", L"�G���[", MB_OK);
        return;
    }

    s_eventList.erase(eventName);
}


// -------------------------------------
// �C�x���g���X�g���N���A����
// -------------------------------------
void EventMessenger::ClearEventList()
{
    s_eventList.clear();
}
