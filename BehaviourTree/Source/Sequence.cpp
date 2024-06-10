#include "pch.h"

#include "BehaviourTree/Header/Sequence.h"

// シーケンスにアクションを追加する
void Sequence::addAction(Action* action) {
    actions.push_back(action);
}

// シーケンスのアクションを順次実行する
bool Sequence::execute() {
    for (Action* action : actions) {
        if (!action->execute()) {
            return false;
        }
    }
    return true;
}
