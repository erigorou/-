#pragma once
#include "BehaviourTree/Header/Sequence.h"
#include "BehaviourTree/Header/Selector.h"
#include "BehaviourTree/Header/IfDecorator.h"
#include "BehaviourTree/Header/Action.h"

// ビヘイビアツリーのルートクラス
class BehaviorTree {
public:
    BehaviorTree();
    bool run();

private:
    Sequence* root;
};
