#include "pch.h"

#include "BehaviourTree/Header/Action.h"
#include <iostream>


// -------------------------------
//
// Enemy�̃X�e�[�g���������ށB
//
// -------------------------------


// �֎q���痧�A�N�V�����̎��s
// 
bool Action1::execute() {
    std::cout << "Executing Action: " << name() << std::endl;
    return true;
}

// ���̋@�Ɉړ�����A�N�V�����̎��s
bool Action2::execute() {
    std::cout << "Executing Action: " << name() << std::endl;
    return true;
}

// �����𔃂��A�N�V�����̎��s
bool Action3::execute() {
    std::cout << "Executing Action: " << name() << std::endl;
    return true;
}

// ���𔃂��A�N�V�����̎��s
bool Action4::execute() {
    std::cout << "Executing Action: " << name() << std::endl;
    return true;
}

// ���ȂɈړ�����A�N�V�����̎��s
bool Action5::execute() {
    std::cout << "Executing Action: " << name() << std::endl;
    return true;
}

// �֎q�ɍ���A�N�V�����̎��s
bool Action6::execute() {
    std::cout << "Executing Action: " << name() << std::endl;
    return true;
}
