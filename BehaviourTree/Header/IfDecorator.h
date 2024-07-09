#pragma once
#include "Action.h"

// �����f�R���[�^�[�̃N���X
class IfDecorator : public Action {
public:
    // �R���X�g���N�^
    IfDecorator(Action* action, int threshold) 
        :
        action(action),
        threshold(threshold) 
    {
    }

    // ����
    bool execute() override;


    std::string name() override { return "�����f�R���[�^�["; }

private:
    Action* action;
    int threshold;
};
