#include "pch.h"

#include "BehaviourTree/Header/BehaviorTree.h"
#include <iostream>

// �r�w�C�r�A�c���[���\�z����
BehaviorTree::BehaviorTree() {
    root = new Sequence();

    // �A�N�V�����̍쐬
    Action* action1 = new Action1();
    Action* action2 = new Action2();
    Action* action3 = new Action3();
    Action* action4 = new Action4();
    Action* action5 = new Action5();
    Action* action6 = new Action6();

    // �f�R���[�^�[�t���A�N�V����
    IfDecorator* ifDecorator = new IfDecorator(action2, 200);

    // �Z���N�^�[�̍쐬
    Selector* selector = new Selector();
    selector->addAction(action3);
    selector->addAction(action4);

    // �V�[�P���X�ɃA�N�V������ǉ�
    root->addAction(action1);
    root->addAction(ifDecorator);
    root->addAction(selector);
    root->addAction(action5);
    root->addAction(action6);
}


// �r�w�C�r�A�c���[�����s����
bool BehaviorTree::run()
{
    return root->execute();
}
