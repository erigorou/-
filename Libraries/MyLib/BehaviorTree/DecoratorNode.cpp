// ----------------------------------------------------------------------------------
//
// 名前:	DecoratorNode.cpp
// 説明:    ビヘイビアツリーの条件判定ノード
//			条件を通れば子のステートを返す
//			通らなければFailureを返す
// 作成:	池田桜輔
// 日付:    2025/06/02
//
// ----------------------------------------------------------------------------------

#pragma once
#include "pch.h"
#include "DecoratorNode.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="fun">登録する関数</param>
DecoratorNode::DecoratorNode(std::function<bool(float)> fun)
	: 
	m_childNode{},
	m_condition{}
{
	// 条件を判定する関数を登録
	m_condition = fun;
	assert(m_condition);
}


/// <summary>
/// デストラクタ
/// </summary>
DecoratorNode::~DecoratorNode()
{
	Finalize();
}


/// <summary>
/// 初期化処理
/// </summary>
void DecoratorNode::Initialize()
{
}


/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
/// <returns>実行結果</returns>
IBehaviorNode::NodeState DecoratorNode::Update(float elapsedTime)
{
	if (!m_condition(elapsedTime))
	{
		return IBehaviorNode::NodeState::FAILURE;
	}

	// 子ノードの実行
	return m_childNode->Update(elapsedTime);
}


/// <summary>
/// 削除処理
/// </summary>
void DecoratorNode::Finalize()
{
}

/// <summary>
/// 子ノードの追加
/// </summary>
/// <param name="node"></param>
void DecoratorNode::AddNode(std::unique_ptr<IBehaviorNode> node)
{
	// 子ノードの初期化
	node->Initialize();
	// 子ノードを登録
	m_childNode = std::move(node);
}