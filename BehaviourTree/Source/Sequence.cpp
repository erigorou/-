#include "pch.h"

#include "BehaviourTree/Header/Sequence.h"

/// <summary>
/// シーケンスにアクションを追加する関数
/// </summary>
/// <param name="action">追加したいアクションのクラス</param>

void Sequence::addAction(Action* action)
{
	// アクションを追加する
	actions.push_back(action);
}

/// <summary>
/// シーケンス内のアクションを順次実行する関数
/// </summary>
/// <returns>成功 or 失敗</returns>

bool Sequence::execute()
{
	// アクションをすべて実行する
	for (Action* action : actions)
	{
		if (!action->execute())
		{
			// アクションが成功してないなら：失敗
			return false;
		}
	}
	// ：成功
	return true;
}