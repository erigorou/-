#pragma once
#include "BehaviourTree/Header/Sequence.h"
#include "BehaviourTree/Header/Selector.h"
#include "BehaviourTree/Header/IfDecorator.h"
#include "BehaviourTree/Header/Action.h"

// �r�w�C�r�A�c���[�̃��[�g�N���X
class BehaviorTree {
public:
    BehaviorTree();
    bool run();

private:
    Sequence* root;
};
