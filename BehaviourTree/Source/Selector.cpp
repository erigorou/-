#include "pch.h"

#include "BehaviourTree/Header/Selector.h"

// セレクターにアクションを追加する
void Selector::addAction(Action* action)
{
	actions.push_back(action);
}

// ランダムにアクションを選択して実行する
bool Selector::execute()
{
	// アクションがない場合は失敗
	if (actions.empty()) return false;
	// アクションをランダムに選択
	int index = rand() % actions.size();
	// 選んだアクションを実行
	return actions[index]->execute();
}