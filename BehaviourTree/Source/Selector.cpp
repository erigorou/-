#include "pch.h"

#include "BehaviourTree/Header/Selector.h"

// �Z���N�^�[�ɃA�N�V������ǉ�����
void Selector::addAction(Action* action) {
    actions.push_back(action);
}

// �����_���ɃA�N�V������I�����Ď��s����
bool Selector::execute() {
    if (actions.empty()) return false;
    int index = rand() % actions.size();
    return actions[index]->execute();
}