// ----------------------------------------------------------------------------------
//
// 名前:	BossBehavior.h
// 説明:    ボスのビヘイビアツリーを管理する
//			BehaviorTreeのルートノードとなるクラス
// 作成:	池田桜輔
// 日付:    2025/06/02
//
// ----------------------------------------------------------------------------------

// インクルード
#pragma once
#include "Libraries/MyLib/BehaviorTree/Interface/IcompositeNode.h"

// 前方宣言
class Root;
class Boss;
class ActionNode;
class ExecutionNode;
class DecoratorNode;
class BossCondition;

/// <summary>
/// ボスのビヘイビアツリーを管理するクラス
/// </summary>
class BossBehavior final
{
	//------------------------
	// 固定値
	// -----------------------
public:


	//------------------------
	// アクセサ
	// -----------------------
public:
	// ビヘイビアを動かす
	void RunBehavior();

	//------------------------
	// メンバ関数(公開)
	// -----------------------
public:
	// コンストラクタ
	BossBehavior(Boss* boss);
	// デストラクタ
	~BossBehavior();
	// 初期化処理
	void Initialize();
	// ビヘイビアを動かす
	void TickBehavior(float elapsedTime);
	// 終了処理
	void Finalize();

	// -----------------------
	// メンバ関数(非公開)
	// -----------------------
private:
	// ビヘイビアツリーを作成
	void CreateBehaviorTree();


	//_________________________________
	// 
	// アクションノード
	//_________________________________
	
	// 待機状態
	std::unique_ptr<ActionNode> CreateIdleActionNode();
	// 降り下ろし攻撃
	std::unique_ptr<ActionNode> CreateAttackActionNode();
	// 薙ぎ払い攻撃
	std::unique_ptr<ActionNode> CreateSweepActionNode();
	// ダッシュ攻撃
	std::unique_ptr<ActionNode> CreateDashActionNode();
	// 近づく
	std::unique_ptr<ActionNode> CreateApproachActionNode();

	//_________________________________
	//
	// デコレーター
	//_________________________________

	// ボスのHPが半分以上の時に実行するデコレーター
	std::unique_ptr<DecoratorNode> CreateHalfHpDecorator();
	// ボスの攻撃範囲内にいる時に実行するデコレーター
	std::unique_ptr<DecoratorNode> CreateInAttackRangeDecorator();
	// 2分の1で成功するデコレーター
	std::unique_ptr<DecoratorNode> CreateHalfSuccessDecorator();



	// -----------------------
	// メンバ変数
	// -----------------------
private:
	// ボス
	Boss* m_boss;

	// ルートノード
	std::unique_ptr<ICompositeNode> m_rootNode;
	// 実行ノードクラス
	std::unique_ptr<ExecutionNode> m_executionNode;
	// 条件ノードクラス
	std::unique_ptr<BossCondition> m_conditionNode;

	// 現在の状態
	IBehaviorNode::NodeState m_currentState;
};