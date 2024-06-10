#include "pch.h"

#include "BehaviourTree/Header/Sequence.h"

// �V�[�P���X�ɃA�N�V������ǉ�����
void Sequence::addAction(Action* action) {
    actions.push_back(action);
}

// �V�[�P���X�̃A�N�V�������������s����
bool Sequence::execute() {
    for (Action* action : actions) {
        if (!action->execute()) {
            return false;
        }
    }
    return true;
}
