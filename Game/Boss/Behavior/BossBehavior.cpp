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
/// ビヘイビアツリーの生成
/// </summary>
void BossBehavior::CreateBehaviorTree()
{
	// 実行ノードの作成
	m_executionNode = std::make_unique<ExecutionNode>();

	// 条件ノードの作成
	m_conditionNode = std::make_unique<BossCondition>(m_boss);

	//___________________________________________
	//
	// HPが半分以上の場合のビヘイビアの構築
	//___________________________________________

	// ★攻撃★
	// 1/2　→　叩きつけ
	auto attackDecoratorNode = CreateHalfSuccessDecorator();
	attackDecoratorNode->AddNode(CreateAttackActionNode());
	// 1/2　→　薙ぎ払い
	auto sweepDecoratorNode = CreateHalfSuccessDecorator();
	sweepDecoratorNode->AddNode(CreateSweepActionNode());
	// 待機
	auto idleActionNode = CreateIdleActionNode();

	// 攻撃セレクター
	auto attackSelectorNode = std::make_unique<SelectorNode>();
	// 叩きつけデコレータを追加
	attackSelectorNode->AddNode(std::move(attackDecoratorNode));
	// 薙ぎ払いデコレータを追加
	attackSelectorNode->AddNode(std::move(sweepDecoratorNode));
	// 待機アクションを追加
	attackSelectorNode->AddNode(std::move(idleActionNode));



	// ★近づく★
	// 1/2　→　近づく
	auto approachActionNode = CreateHalfHpDecorator();
	approachActionNode->AddNode(CreateApproachActionNode());
	// ダッシュ攻撃
	auto dashActionNode = CreateDashActionNode();

	// 近づくセレクター
	auto approachSelectorNode = std::make_unique<SelectorNode>();
	// 近づくアクションを追加
	approachSelectorNode->AddNode(std::move(approachActionNode));
	// ダッシュ攻撃アクションを追加
	approachSelectorNode->AddNode(std::move(dashActionNode));


	// ★HPが半分以上★
	// 攻撃範囲内にいるなら攻撃セレクター、違うなら近づくセレクターを実行するセレクター
	auto inAttackRangeDecoratorNode = CreateInAttackRangeDecorator();
	inAttackRangeDecoratorNode->AddNode(std::move(attackSelectorNode));

	auto halfHpOverSelectorNode = std::make_unique<SelectorNode>();
	// 攻撃範囲内にいるなら攻撃セレクターを追加
	halfHpOverSelectorNode->AddNode(std::move(inAttackRangeDecoratorNode));
	// 攻撃範囲外なら近づくセレクターを追加
	halfHpOverSelectorNode->AddNode(std::move(approachSelectorNode));
	


	//___________________________________________
	//
	// HPが半分以下の場合のビヘイビアの構築
	//___________________________________________

	// ★攻撃★
	
	// 薙ぎ払いアクションを追加
	sweepDecoratorNode = CreateHalfSuccessDecorator();
	sweepDecoratorNode->AddNode(CreateSweepActionNode());
	// 叩きつけアクションを追加
	attackDecoratorNode = CreateHalfSuccessDecorator();
	attackDecoratorNode->AddNode(CreateAttackActionNode());

	// 攻撃セレクター
	auto attackSelectorNode2 = std::make_unique<SelectorNode>();
	// 薙ぎ払いデコレータを追加
	attackSelectorNode2->AddNode(std::move(sweepDecoratorNode));
	// 叩きつけデコレータを追加
	attackSelectorNode2->AddNode(std::move(attackDecoratorNode));


	// ★近づく★
	// 近づくアクションを追加
	auto approachDecoratorNode = CreateHalfSuccessDecorator();
	approachDecoratorNode->AddNode(CreateApproachActionNode());

	// ダッシュ攻撃アクションを追加
	auto dashDecoratorNode = CreateHalfSuccessDecorator();
	dashDecoratorNode->AddNode(CreateDashActionNode());


	// ★HPが半分以下★
	// 攻撃範囲内にいるなら攻撃セレクター、違うなら近づくセレクターを実行するセレクター
	auto inAttackRangeDecoratorNode2 = CreateInAttackRangeDecorator();
	inAttackRangeDecoratorNode2->AddNode(std::move(attackSelectorNode2));

	// HP半分以下のセレクター
	auto halfHpUnderSelectorNode = std::make_unique<SelectorNode>();
	// 攻撃範囲内にいるなら攻撃セレクターを追加
	halfHpUnderSelectorNode->AddNode(std::move(inAttackRangeDecoratorNode2));
	// 攻撃範囲外なら近づくセレクターを追加
	halfHpUnderSelectorNode->AddNode(std::move(approachDecoratorNode));



	//_____________________________________________________
	//
	// HPが半分以上か以下かで分岐するノードの作成
	//_____________________________________________________

	// ボスのHPが半分以上かどうかで分岐するセレクター
	auto selectorNode = std::make_unique<SelectorNode>();
	selectorNode->AddNode(std::move(halfHpOverSelectorNode));
	selectorNode->AddNode(std::move(halfHpUnderSelectorNode));

	// ルートノードの作成
	m_rootNode = std::make_unique<Root>();
	// ルートノードにセレクターを追加
	m_rootNode->AddNode(std::move(selectorNode));


	// 状態の初期化
	m_currentState = IBehaviorNode::NodeState::FAILURE;
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
// ★ == コンディションを持ったデコレータノードを作成するクラス群 == ★
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
