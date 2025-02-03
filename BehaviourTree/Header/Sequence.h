#pragma once
#include "Action.h"
#include <vector>

// �V�[�P���X�m�[�h�̃N���X
class Sequence : public Action {
public:
	// �V�[�P���X�ɃA�N�V������ǉ�����
	void addAction(Action* action);
	bool execute() override;
	std::string name() override { return "�V�[�P���X"; }

private:
	std::vector<Action*> actions;
};
