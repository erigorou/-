// ----------------------------------------------------------------------------------
//
// 名前:	BossBehavior.h
// 説明:    ボスのビヘイビアツリーを管理する
//			BehaviorTreeのルートノードとなるクラス
// 作成:	池田桜輔
// 日付:    2025/06/02
//
// ----------------------------------------------------------------------------------

#pragma once
#include "pch.h"
#include "../Boss.h"
#include "BossBehavior.h"
#include "ExecutionNode.h"
#include "BossCondition.h"

// BTのノード
#include "Libraries/MyLib/BehaviorTree/Node/ActionNode/ActionNode.h"
#include "Libraries/MyLib/BehaviorTree/Node/DecoratorNode/DecoratorNode.h"
#include "Libraries/MyLib/BehaviorTree/Node/RootNode/Root.h"
#include "Libraries/MyLib/BehaviorTree/Node/SelectorNode/SelectorNode.h"
#include "Libraries/MyLib/BehaviorTree/Node/SequenceNode/SequenceNode.h"

// 読み込み
#include "Libraries/MyLib/BehaviorTree/JsonLoader/BehaviorNodeLoader.h"


/// <summary>
/// コンストラクタ
/// </summary>
BossBehavior::BossBehavior(Boss* boss)
	: 
	m_boss(boss)
{
}


/// <summary>
/// デストラクタ
/// </summary>
BossBehavior::~BossBehavior()
{
	Finalize();
}


/// <summary>
/// 初期化処理
/// </summary>
void BossBehavior::Initialize()
{
	// ビヘイビアツリーの生成
	CreateBehaviorTree();
}


/// <summary>
/// 終了処理
/// </summary>
void BossBehavior::Finalize()
{
	// ルートノードの削除
	m_rootNode.reset();
	// 実行ノードの削除
	m_executionNode.reset();
	// 条件ノードの削除
	m_conditionNode.reset();
}


/// <summary>
/// 関数マップを作成
/// </summary>
void BossBehavior::CreateFunctionMap()
{
	// アクションノードのマップ
	m_actionMap = {
		{"Idle", std::bind(&ExecutionNode::BossEnemyIdleAction, m_executionNode.get())},
		{"Attack", std::bind(&ExecutionNode::BossEnemyAttackAction, m_executionNode.get())},
		{"Sweep", std::bind(&ExecutionNode::BossEnemySweepAttackAction, m_executionNode.get())},
		{"Dash", std::bind(&ExecutionNode::BossEnemyDashAttackAction, m_executionNode.get())},
		{"Approach", std::bind(&ExecutionNode::BossEnemyApproachAction, m_executionNode.get())}
	};

	// 条件ノードのマップ
	m_conditionMap = {
		{"IsHpOverHalf", std::bind(&BossCondition::IsHpOverHalf, m_conditionNode.get())},
		{"IsInCloseRange", std::bind(&BossCondition::IsInCloseRange, m_conditionNode.get())},
		{"IsHalfSuccess", std::bind(&BossCondition::IsHalfSuccess, m_conditionNode.get())}
	};
}

/// <summary>
/// ビヘイビアツリーの生成
/// </summary>
void BossBehavior::CreateBehaviorTree()
{
	// 状態の初期化
	m_currentState = IBehaviorNode::NodeState::FAILURE;

	// 実行ノードの作成
	m_executionNode = std::make_unique<ExecutionNode>();
	// 条件ノードの作成
	m_conditionNode = std::make_unique<BossCondition>(m_boss);

	// 関数マップの作成
	CreateFunctionMap();
	// JSONからBTを構築
	m_rootNode = BehaviorNodeLoader::LoadFromFile("Resources/Jsons/Behavior/BossBehaviorNode.json", m_actionMap, m_conditionMap);
}



/// <summary>
/// ビヘイビアを動かす
/// </summary>
void BossBehavior::RunBehavior()
{
	switch (m_currentState)
	{
	case IBehaviorNode::NodeState::SUCCESS:
		break;
	case IBehaviorNode::NodeState::FAILURE:
		m_rootNode->Update();
		break;
	default:
		break;
	}
}



// ----------------------------------------------------------------------



//////////////////////////////////////////////////
// 
// ★ == アクションノードを作成するクラス群 == ★
// 
//////////////////////////////////////////////////


/// <summary>
/// 待機アクション作成
/// </summary>
/// <returns>アクションノード</returns>
std::unique_ptr<ActionNode> BossBehavior::CreateIdleActionNode()
{
	// 待機アクションノードを作成
	return std::make_unique<ActionNode>(std::bind(&ExecutionNode::BossEnemyIdleAction, m_executionNode.get()));
}


/// <summary>
/// 叩きつけアクションノードを作成
/// </summary
/// <returns>アクションノード</returns>
std::unique_ptr<ActionNode> BossBehavior::CreateAttackActionNode()
{
	// 叩きつけアクションノードを作成
	return std::make_unique<ActionNode>(std::bind(&ExecutionNode::BossEnemyAttackAction, m_executionNode.get()));
}


/// <summary>	
/// 薙ぎ払いアクションノードを作成
/// </summary>
/// <returns>アクションノード</returns>
std::unique_ptr<ActionNode> BossBehavior::CreateSweepActionNode()
{
	// 薙ぎ払いアクションノードを作成
	return std::make_unique<ActionNode>(std::bind(&ExecutionNode::BossEnemySweepAttackAction, m_executionNode.get()));
}


/// <summary>
/// ダッシュ攻撃アクションノードを作成
/// </summary>
/// <returns>アクションノード</returns>
std::unique_ptr<ActionNode> BossBehavior::CreateDashActionNode()
{
	// ダッシュ攻撃アクションノードを作成
	return std::make_unique<ActionNode>(std::bind(&ExecutionNode::BossEnemyDashAttackAction, m_executionNode.get()));
}


/// <summary>
/// 近づくアクションノードを作成
/// </summary>
/// <returns>アクションノード</returns>
std::unique_ptr<ActionNode> BossBehavior::CreateApproachActionNode()
{
	// 近づくアクションノードを作成
	return std::make_unique<ActionNode>(std::bind(&ExecutionNode::BossEnemyApproachAction, m_executionNode.get()));
}



// ----------------------------------------------------------------------



///////////////////////////////////////////////////////////////////////
// 
// ★ == コンディションを持ったデコレータノードを作成するクラス群 == ★
//
///////////////////////////////////////////////////////////////////////


/// <summary>
/// 2分の1で成功するデコレーターを作成
/// </summary>
/// <returns>デコレーター</returns>
std::unique_ptr<DecoratorNode> BossBehavior::CreateHalfHpDecorator()
{
	return std::make_unique<DecoratorNode>(std::bind(&BossCondition::IsHalfSuccess, m_conditionNode.get()));
}


/// <summary>
/// 攻撃範囲内にいる時に実行するデコレーターを作成
/// </summary>
/// <returns>デコレーター</returns>
std::unique_ptr<DecoratorNode> BossBehavior::CreateInAttackRangeDecorator()
{
	return std::make_unique<DecoratorNode>(std::bind(&BossCondition::IsInCloseRange, m_conditionNode.get()));
}


/// <summary>
/// ボスのHPが半分以上の時に実行するデコレーターを作成
/// </summary>
/// <returns>デコレーター</returns>
std::unique_ptr<DecoratorNode> BossBehavior::CreateHalfSuccessDecorator()
{
	return std::make_unique<DecoratorNode>(std::bind(&BossCondition::IsHpOverHalf, m_conditionNode.get()));
}
