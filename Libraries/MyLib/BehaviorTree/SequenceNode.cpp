// ----------------------------------------------------------------------------------
//
// 名前:	SequenceNode.h
// 説明:    ビヘイビアツリーのノード
//			順に個を実行する
//			全ての子が実行できるとSuccess
//			子が失敗したらすぐにFailureを返す 
// 作成:	池田桜輔
// 日付:    2025/06/02
//
// ----------------------------------------------------------------------------------

#include "pch.h"
#include "SequenceNode.h"


/// <summary>
/// コンストラクタ
/// </summary>
SequenceNode::SequenceNode()
	: m_childNodes{},
	m_runningNode{}
{
}


/// <summary>
/// デストラクタ
/// </summary>
SequenceNode::~SequenceNode()
{
	Finalize();
}


/// <summary>
/// 初期化処理
/// </summary>
void SequenceNode::Initialize()
{
	// 初期化処理は特にない
}


/// <summary>
/// 更新処理
/// </summary>
/// <returns>実行結果</returns>
IBehaviorNode::NodeState SequenceNode::Update()
{
	// 一つずつ検証する
	for (auto& child : m_childNodes)
	{
		switch (child->Update())
		{
		// 成功　→ 次の子ノードへ
		case IBehaviorNode::NodeState::SUCCESS:
			continue;

		// 失敗　→　終了
		case IBehaviorNode::NodeState::FAILURE:
			return IBehaviorNode::NodeState::FAILURE;

		// 実行中　→　実行中のノードを保持
		case IBehaviorNode::NodeState::RUNNING:
			m_runningNode = child.get();
			return IBehaviorNode::NodeState::RUNNING;

		// それ以外
		default:
			break;
		}
	}

	// 全ての子ノードが成功した場合は成功を返す
	return IBehaviorNode::NodeState::SUCCESS;
}


/// <summary>
/// 終了処理
/// </summary>
void SequenceNode::Finalize()
{
	// 子ノードの解放
	m_childNodes.clear();
	m_runningNode = nullptr;
}


/// <summary>
/// 子ノードの追加
/// </summary>
/// <param name="child">子ノード</param>
void SequenceNode::AddNode(std::unique_ptr<IBehaviorNode> child)
{
	// 初期化
	child->Initialize();
	// 登録
	m_childNodes.emplace_back(std::move(child));
}