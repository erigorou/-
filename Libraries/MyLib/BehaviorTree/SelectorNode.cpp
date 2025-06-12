// ----------------------------------------------------------------------------------
//
// 名前:	SelectorNode.h
// 説明:	ビヘイビアツリーのノード
//			成功する子が見つかるまで子を実行する
//			見つかった瞬間Successを返す
//			全ての子が失敗したらFailureを返す
// 作成:	池田桜輔
// 日付:    2025/06/02
//
// ----------------------------------------------------------------------------------

#include "pch.h"
#include "SelectorNode.h"


/// <summary>
/// コンストラクタ
/// </summary>
SelectorNode::SelectorNode()
	: 
	m_childNodes{},
	m_runningNode{}
{
}


/// <summary>
/// デストラクタ
/// </summary>
SelectorNode::~SelectorNode()
{
	Finalize();
}


/// <summary>
/// 初期化処理
/// </summary>
void SelectorNode::Initialize()
{
}


/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">1Fの経過時間</param>
/// <returns>実行結果</returns>
IBehaviorNode::NodeState SelectorNode::Update(float elapsedTime)
{
	// 一つずつ検証する
	for (auto& child : m_childNodes)
	{
		switch (child->Update(elapsedTime))
		{
		// 成功した場合
		case IBehaviorNode::NodeState::SUCCESS:
			// 成功したらそのまま返す
			return IBehaviorNode::NodeState::SUCCESS;
		
		// 実行中の場合
		case IBehaviorNode::NodeState::RUNNING:
			// 実行中のノードを保持
			m_runningNode = child.get();
			return IBehaviorNode::NodeState::RUNNING;
		
		// 失敗した場合
		case IBehaviorNode::NodeState::FAILURE:
			// もしかすると他の子ノードが成功するかもしれないので、何もしない
			break;
		}
	}

	// 1つも成功しなかった場合は失敗を返す
	return IBehaviorNode::NodeState::FAILURE;
}


/// <summary>
/// 終了処理
/// </summary>
void SelectorNode::Finalize()
{
	// 子ノードの解放
	m_childNodes.clear();
	m_runningNode = nullptr;
}


/// <summary>
/// 子ノードの追加
/// </summary>
/// <param name="childNode">追加する子ノード</param>
void SelectorNode::AddNode(std::unique_ptr<IBehaviorNode> node)
{
	// 子ノードの初期化
	node->Initialize();

	// 子ノードを追加
	m_childNodes.emplace_back(std::move(node));
}