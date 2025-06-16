// ----------------------------------------------------------------------------------
//
// 名前:	SelectorNode.h
// 説明:	ビヘイビアツリーのノード
//			成功する子が見つかるまで子を実行する
//			見つかった瞬間Successを返す
//			全ての子が失敗したらFailureを返す
// 作成:	池田桜輔
// 日付:    2025/06/02
//
// ----------------------------------------------------------------------------------
#pragma once
#include "Interface/ICompositeNode.h"

/// <summary>
///ビヘイビアツリーのセレクターノード
/// </summary>
class SelectorNode final : public ICompositeNode
{
	//------------------------
	// メンバ関数(公開)
	// -----------------------
public:
	 // コンストラクタ
	SelectorNode();
	// デストラクタ
	~SelectorNode() override;
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
	// 子ノード配列
	std::vector<std::unique_ptr<IBehaviorNode>> m_childNodes;

	// 実行中ノードの格納変数
	IBehaviorNode* m_runningNode = nullptr;
};