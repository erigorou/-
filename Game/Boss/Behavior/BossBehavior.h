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
class DecorationNode;


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

	// ★ ================================= ★通常
	// 待機状態
	void CreateIdleActionNode();

	// ★================================= ★攻撃系
	// 降り下ろし攻撃
	void CreateAttackActionNode();
	// 薙ぎ払い攻撃
	void CreateSweepActionNode();
	// ダッシュ攻撃
	void CreateDashActionNode();

	// ★================================ ★移動系
	// 近づく
	void CreateApproachActionNode();


	// -----------------------
	// メンバ変数
	// -----------------------
private:
	// ボスのポインタ
	Boss* m_boss = nullptr;

	// ルートノード
	std::unique_ptr<ICompositeNode> m_rootNode;

	// 現在の状態
	IBehaviorNode::NodeState m_currentState;
};