#include "pch.h"

#include "BehaviourTree/Header/Selector.h"

// セレクターにアクションを追加する
void Selector::addAction(Action* action) {
    actions.push_back(action);
}

// ランダムにアクションを選択して実行する
bool Selector::execute() {
    if (actions.empty()) return false;
    int index = rand() % actions.size();
    return actions[index]->execute();
}