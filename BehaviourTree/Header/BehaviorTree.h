#pragma once
#include "BehaviourTree/Header/Sequence.h"
#include "BehaviourTree/Header/Selector.h"
#include "BehaviourTree/Header/IfDecorator.h"
#include "BehaviourTree/Header/Action.h"

// ビヘイビアツリーのルートクラス
class BehaviorTree {
public:
    BehaviorTree();

    /// <summary>
    /// rootに構築されたBTを実行する
    /// </summary>
    /// <returns>成功 or 失敗</returns>
    bool run();

private:
    Sequence* root;
};
