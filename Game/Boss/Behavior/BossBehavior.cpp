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

#include "Libraries/MyLib/BehaviorTree/ActionNode.h"
#include "Libraries/MyLib/BehaviorTree/DecoratorNode.h"
#include "Libraries/MyLib/BehaviorTree/Root.h"
#include "Libraries/MyLib/BehaviorTree/SelectorNode.h"
#include "Libraries/MyLib/BehaviorTree/SequenceNode.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="boss"></param>
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
}


/// <summary>
/// ビヘイビアツリーの生成
/// </summary>
void BossBehavior::CreateBehaviorTree()
{
	// 実行ノードの作成
	m_executionNode = std::make_unique<ExecutionNode>();

	// 条件ノードの作成
	m_conditionNode = std::make_unique<BossCondition>();

	//___________________________________________
	//
	// HPが半分以上の場合のビヘイビアの構築
	//___________________________________________

	// 叩きつけ攻撃のデコレーター
	auto attackDecoratorNode = 








	// 状態の初期化
	m_currentState = IBehaviorNode::NodeState::FAILURE;
}







//////////////////////////////////////////////////
// ★ == アクションノードを作成するクラス群 == ★
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
	return std::make_unique<ActionNode>(
		std::bind(&ExecutionNode::BossEnemyAttackAction, m_executionNode.get()));
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




///////////////////////////////////////////////////////////////////////
// ★ == コンディションを持ったデコレータノードを作成するクラス群 == ★
///////////////////////////////////////////////////////////////////////

/// <summary>
/// 2分の1で成功するデコレーターを作成
/// </summary>
/// <returns>デコレーター</returns>
std::unique_ptr<DecoratorNode> BossBehavior::CreateHalfHpDecorator()
{
	return std::unique_ptr<DecoratorNode>();
}


/// <summary>
/// 攻撃範囲内にいる時に実行するデコレーターを作成
/// </summary>
/// <returns>デコレーター</returns>
std::unique_ptr<DecoratorNode> BossBehavior::CreateInAttackRangeDecorator()
{
	return std::unique_ptr<DecoratorNode>();
}


/// <summary>
/// ボスのHPが半分以上の時に実行するデコレーターを作成
/// </summary>
/// <returns>デコレーター</returns>
std::unique_ptr<DecoratorNode> BossBehavior::CreateHalfSuccessDecorator()
{
	return std::unique_ptr<DecoratorNode>();
}
