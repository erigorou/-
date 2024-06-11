#pragma once
#include <string>

// �A�N�V�����̊��N���X
class Action {
public:
    // �A�N�V���������s���鏃�����z�֐�
    virtual bool execute() = 0;
    // �A�N�V�����̖��O���擾���鏃�����z�֐�
    virtual std::string name() = 0;
};

// �����@������������������������
// composite�p�^�[�� **************************


// �֎q���痧�A�N�V����
class Action1 : public Action {
public:
    bool execute() override;
    std::string name() override { return "�֎q���痧��"; }
};

// ���̋@�Ɉړ�����A�N�V����
class Action2 : public Action {
public:
    bool execute() override;
    std::string name() override { return "���̋@�Ɉړ�����"; }
};

// �����𔃂��A�N�V����
class Action3 : public Action {
public:
    bool execute() override;
    std::string name() override { return "�����𔃂�"; }
};

// ���𔃂��A�N�V����
class Action4 : public Action {
public:
    bool execute() override;
    std::string name() override { return "���𔃂�"; }
};

// ���ȂɈړ�����A�N�V����
class Action5 : public Action {
public:
    bool execute() override;
    std::string name() override { return "���ȂɈړ�����"; }
};

// �֎q�ɍ���A�N�V����
class Action6 : public Action {
public:
    bool execute() override;
    std::string name() override { return "�֎q�ɍ���"; }
};
