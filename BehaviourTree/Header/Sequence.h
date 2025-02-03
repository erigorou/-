#pragma once
#include "Action.h"
#include <vector>

// シーケンスノードのクラス
class Sequence : public Action {
public:
	// シーケンスにアクションを追加する
	void addAction(Action* action);
	bool execute() override;
	std::string name() override { return "シーケンス"; }

private:
	std::vector<Action*> actions;
};
