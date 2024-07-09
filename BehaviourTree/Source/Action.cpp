#include "pch.h"

#include "BehaviourTree/Header/Action.h"
#include <iostream>


// -------------------------------
//
// Enemyのステートを書き込む。
//
// -------------------------------


// 椅子から立つアクションの実行
// 
bool Action1::execute() {
    std::cout << "Executing Action: " << name() << std::endl;
    return true;
}

// 自販機に移動するアクションの実行
bool Action2::execute() {
    std::cout << "Executing Action: " << name() << std::endl;
    return true;
}

// 麦茶を買うアクションの実行
bool Action3::execute() {
    std::cout << "Executing Action: " << name() << std::endl;
    return true;
}

// 水を買うアクションの実行
bool Action4::execute() {
    std::cout << "Executing Action: " << name() << std::endl;
    return true;
}

// 自席に移動するアクションの実行
bool Action5::execute() {
    std::cout << "Executing Action: " << name() << std::endl;
    return true;
}

// 椅子に座るアクションの実行
bool Action6::execute() {
    std::cout << "Executing Action: " << name() << std::endl;
    return true;
}
