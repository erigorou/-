// ----------------------------------------------------------------------------------
//
// 名前:	Root.cpp
// 説明:    ビヘイビアツリーのノード
// 			順番に子を実行する
//  		全ての子が実行できるとSuccess
//			子が失敗したらすぐにFailureを返す
// 作成:	池田桜輔
// 日付:    2025/06/02
//
// ----------------------------------------------------------------------------------

#pragma once
#include "pch.h"
#include "Root.h"


/// <summary>
/// コンストラクタ
/// </summary>
Root::Root()
	: m_childNode(nullptr)
{
}

/// <summary>
/// 終了処理
/// </summary>
Root::~Root()
{
	Finalize();
}


/// <summary>
/// 初期化処理
/// </summary>
void Root::Initialize()
{
}


/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime"></param>
/// <returns></returns>
IBehaviorNode::NodeState Root::Update(float elapsedTime)
{
	// 子ノードの実行
	return m_childNode->Update(elapsedTime);
}


/// <summary>
/// 終了処理
/// </summary>
void Root::Finalize()
{
	// 子ノードの解放
	m_childNode.reset();
}


/// <summary>
/// ノードの追加
/// </summary>
/// <param name="node">追加するノード</param>
void Root::AddNode(std::unique_ptr<IBehaviorNode> node)
{
	// 子ノードを初期化
	node->Initialize();

	// 子ノードを登録
	m_childNode = std::move(node);
}