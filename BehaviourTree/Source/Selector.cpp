#include "pch.h"

#include "BehaviourTree/Header/Selector.h"

// �Z���N�^�[�ɃA�N�V������ǉ�����
void Selector::addAction(Action* action)
{
	actions.push_back(action);
}

// �����_���ɃA�N�V������I�����Ď��s����
bool Selector::execute()
{
	// �A�N�V�������Ȃ��ꍇ�͎��s
	if (actions.empty()) return false;
	// �A�N�V�����������_���ɑI��
	int index = rand() % actions.size();
	// �I�񂾃A�N�V���������s
	return actions[index]->execute();
}