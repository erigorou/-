#include "pch.h"

#include "BehaviourTree/Header/IfDecorator.h"
#include <iostream>

// 条件を確認してアクションを実行する
bool IfDecorator::execute()
{
	// ここではデモとして閾値を200と仮定する
	int possession = 200;

	// hのコンストラクタでthresholdに初期値を入れてある
	if (possession >= threshold)
	{
		return action->execute();
	}
	std::cout << "条件を満たしていません: " << threshold << std::endl;
	return false;
}