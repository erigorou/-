#pragma once
#include "Action.h"

// 条件デコレーターのクラス
class IfDecorator : public Action {
public:
    // コンストラクタ
    IfDecorator(Action* action, int threshold) 
        :
        action(action),
        threshold(threshold) 
    {
    }

    // 処理
    bool execute() override;


    std::string name() override { return "条件デコレーター"; }

private:
    Action* action;
    int threshold;
};
