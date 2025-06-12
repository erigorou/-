// ----------------------------------------------------------------------------------
//
// 名前:	ActionNode.cpp
// 説明:    ビヘイビアツリーの実行ノード
// //		子を持たない
// 作成:	池田桜輔
// 日付:    20yy/06/02
//
// ----------------------------------------------------------------------------------

#include "pch.h"
#include "Libraries/MyLib/BehaviorTree/ActionNode.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="action">実行する関数</param>
ActionNode::ActionNode(std::function<NodeState(float)> action)
	: 
	m_executionNode(action)
{
}