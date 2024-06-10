#pragma once
#include "Action.h"

// 条件デコレーターのクラス
class IfDecorator : public Action {
public:
    IfDecorator(Action* action, int threshold) : action(action), threshold(threshold) {}
    bool execute() override;
    std::string name() override { return "条件デコレーター"; }

private:
    Action* action;
    int threshold;
};
