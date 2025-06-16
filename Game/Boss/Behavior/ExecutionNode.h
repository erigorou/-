// ----------------------------------------------------------------------------------
//
// 名前:	ExecutionNode.h
// 説明:    ビヘイビアツリーのノード
//			実行するアクションをまとめたクラス
// 作成:	池田桜輔
// 日付:    2025/06/02
//
// ----------------------------------------------------------------------------------

#pragma once
#include "Libraries/MyLib/BehaviorTree/Interface/IBehaviorNode.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/Messenger/EventList.h"


/// <summary>
/// ビヘイビアツリーのエクセキューションノード
/// </summary>
class ExecutionNode final
{
	//------------------------
	// アクセサ
	// -----------------------
public:
	// ステート変更（敵）：待機
	IBehaviorNode::NodeState BossEnemyIdleAction();
	// ステート変更（敵）：叩きつけ
	IBehaviorNode::NodeState BossEnemyAttackAction();
	// ステート変更（敵）：薙ぎ払い
	IBehaviorNode::NodeState BossEnemySweepAttackAction();
	// ステート変更（敵）：突進
	IBehaviorNode::NodeState BossEnemyDashAttackAction();
	// ステート変更（敵）：近づく
	IBehaviorNode::NodeState BossEnemyApproachAction();

	//------------------------
	// メンバ関数(公開)
	// -----------------------
public:
	// コンストラクタ
	ExecutionNode();
	// デストラクタ
	~ExecutionNode();
};

