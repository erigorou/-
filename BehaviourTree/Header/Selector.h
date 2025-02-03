#pragma once
#include "Action.h"
#include <vector>
#include <cstdlib>

// セレクターノードのクラス
class Selector : public Action {
public:
	// セレクターにアクションを追加する
	void addAction(Action* action);
	// 実行
	bool execute() override;

	std::string name() override { return "セレクター"; }

private:
	std::vector<Action*> actions;
};
