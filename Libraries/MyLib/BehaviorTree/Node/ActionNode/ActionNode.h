// ----------------------------------------------------------------------------------
//
// 名前:	ActionNode.h
// 説明:    ビヘイビアツリーの実行ノード
//			子を持たない
// 作成:	池田桜輔
// 日付:    2025/06/02
//
// ----------------------------------------------------------------------------------

// インクルード
#pragma once
#include "../../Interface/IBehaviorNode.h"
#include <functional>


/// <summary>
/// ビヘイビアツリーの実行ノード
/// </summary>
class ActionNode final : public IBehaviorNode
{
	//------------------------
	// メンバ関数(公開)
	// -----------------------
public:

	// コンストラクタ
	ActionNode(std::function<NodeState()> action);
	// デストラクタ
	~ActionNode() override;
	// 初期化処理
	void Initialize() override {};
	// 更新処理
	NodeState Update() override;
	// 終了処理
	void Finalize() override;


	// -----------------------
	// メンバ変数
	// -----------------------
private:

	// 実行アクション
	std::function<IBehaviorNode::NodeState()> m_executionNode;
};