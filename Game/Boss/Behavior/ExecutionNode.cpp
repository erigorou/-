// ----------------------------------------------------------------------------------
//
// 名前:	ExecutionNode.h
// 説明:    ビヘイビアツリーのノード
//			実行するアクションをまとめたクラス
// 作成:	池田桜輔
// 日付:    2025/06/02
//
// ----------------------------------------------------------------------------------


#include "pch.h"
#include "ExecutionNode.h"
#include "../Boss.h"


/// <summary>
/// コンストラクタ
/// </summary>
ExecutionNode::ExecutionNode()
{
}


/// <summary>
/// デストラクタ
/// </summary>
ExecutionNode::~ExecutionNode()
{}



/// <summary>
/// ステート変更（敵）：待機
/// </summary>
IBehaviorNode::NodeState ExecutionNode::BossEnemyIdleAction()
{
	// ボスの状態を待機に変更
	BossState state = BossState::Idling;
	EventMessenger::Execute(EventList::ChangeBossState, &state);

	return IBehaviorNode::NodeState::SUCCESS;
}


/// <summary>
/// ステート変更（敵）：叩きつけ
/// </summary>
IBehaviorNode::NodeState ExecutionNode::BossEnemyAttackAction()
{
	// ボスの状態を攻撃に変更
	BossState state = BossState::Attacking;
	EventMessenger::Execute(EventList::ChangeBossState, &state);

	return IBehaviorNode::NodeState::SUCCESS;
}


/// <summary>
/// ステート変更（敵）：薙ぎ払い
/// </summary>
IBehaviorNode::NodeState ExecutionNode::BossEnemySweepAttackAction()
{
	// ボスの状態を薙ぎ払いに変更
	BossState state = BossState::Sweeping;
	EventMessenger::Execute(EventList::ChangeBossState, &state);

	return IBehaviorNode::NodeState::SUCCESS;
}


/// <summary>
/// ステート変更（敵）：突進
/// </summary>
IBehaviorNode::NodeState ExecutionNode::BossEnemyDashAttackAction()
{
	// ボスの状態を突進に変更
	BossState state = BossState::DashAttacking;
	EventMessenger::Execute(EventList::ChangeBossState, &state);

	return IBehaviorNode::NodeState::SUCCESS;
}


/// <summary>
/// ステート変更（敵）：近づく
/// </summary>
IBehaviorNode::NodeState ExecutionNode::BossEnemyApproachAction()
{
	// ボスの状態を近づくに変更
	BossState state = BossState::Approaching;
	EventMessenger::Execute(EventList::ChangeBossState, &state);

	return IBehaviorNode::NodeState::SUCCESS;
}