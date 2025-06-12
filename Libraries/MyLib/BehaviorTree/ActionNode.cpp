// ----------------------------------------------------------------------------------
//
// 名前:	ActionNode.cpp
// 説明:    ビヘイビアツリーの実行ノード
// //		子を持たない
// 作成:	池田桜輔
// 日付:    2026/06/02
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


/// <summary>
/// デストラクタ
/// </summary>
ActionNode::~ActionNode()
{
	Finalize();
}


/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">1Fの経過時間</param>
/// <returns>登録された実行ノードの結果</returns>
IBehaviorNode::NodeState ActionNode::Update(float elapsedTime)
{
	return m_executionNode(elapsedTime);
}

/// <summary>
/// 終了処理
/// </summary>
void ActionNode::Finalize()
{
	// 実行ノードの解放
	m_executionNode = nullptr;
}