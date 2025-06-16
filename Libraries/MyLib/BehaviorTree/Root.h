// ----------------------------------------------------------------------------------
//
// 名前:	Root.h
// 説明:    ビヘイビアツリーのノード
// 			順番に子を実行する
//  		全ての子が実行できるとSuccess
//			子が失敗したらすぐにFailureを返す
// 作成:	池田桜輔
// 日付:    2025/06/02
//
// ----------------------------------------------------------------------------------
#pragma once
#include "Interface/IcompositeNode.h"

class Root final : public ICompositeNode
{
	//------------------------
	// メンバ関数(公開)
	// -----------------------
public:
	// コンストラクタ
	Root();
	// デストラクタ
	~Root() override;
	// 初期化処理
	void Initialize() override;
	// 更新処理
	IBehaviorNode::NodeState Update() override;
	// 終了処理
	void Finalize() override;
	// ノードの追加
	void AddNode(std::unique_ptr<IBehaviorNode> node) override;

	// -----------------------
	// メンバ変数
	// -----------------------
private:
	// 子ノード
	std::unique_ptr<IBehaviorNode> m_childNode;
};