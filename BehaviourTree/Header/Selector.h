#pragma once
#include "Action.h"
#include <vector>
#include <cstdlib>

// �Z���N�^�[�m�[�h�̃N���X
class Selector : public Action {
public:
	// �Z���N�^�[�ɃA�N�V������ǉ�����
	void addAction(Action* action);
	// ���s
	bool execute() override;

	std::string name() override { return "�Z���N�^�["; }

private:
	std::vector<Action*> actions;
};
