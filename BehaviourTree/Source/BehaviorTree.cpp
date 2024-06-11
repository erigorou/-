#include "pch.h"

#include "BehaviourTree/Header/BehaviorTree.h"
#include <iostream>

// ビヘイビアツリーを構築する
BehaviorTree::BehaviorTree() {
    root = new Sequence();

    // アクションの作成
    Action* action1 = new Action1();
    Action* action2 = new Action2();
    Action* action3 = new Action3();
    Action* action4 = new Action4();
    Action* action5 = new Action5();
    Action* action6 = new Action6();

    // デコレーター付きアクション
    IfDecorator* ifDecorator = new IfDecorator(action2, 200);

    // セレクターの作成
    Selector* selector = new Selector();
    selector->addAction(action3);
    selector->addAction(action4);

    // シーケンスにアクションを追加
    root->addAction(action1);
    root->addAction(ifDecorator);
    root->addAction(selector);
    root->addAction(action5);
    root->addAction(action6);
}


// ビヘイビアツリーを実行する
bool BehaviorTree::run()
{
    return root->execute();
}
