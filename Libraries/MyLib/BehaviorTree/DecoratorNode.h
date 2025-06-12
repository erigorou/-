// ----------------------------------------------------------------------------------
//
// 名前:	DecoratorNode.h
// 説明:    ビヘイビアツリーの条件判定ノード
//			条件を通れば子のステートを返す
//			通らなければFailureを返す
// 作成:	池田桜輔
// 日付:    2025/06/02
//
// ----------------------------------------------------------------------------------
// インクルード
#pragma once
#include "Interface/IcompositeNode.h"


/// <summary>
/// ビヘイビアツリーの条件判定ノード
/// </summary>
class DecoratorNode final : public ICompositeNode
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
	DecoratorNode(std::function<bool(float)> fun);
	// デストラクタ
	~DecoratorNode() override;
	// 初期化処理
	void Initialize() override;
	// 更新処理
	IBehaviorNode::NodeState Update(float elapsedTime) override;
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

	// 条件を判定する関数
	std::function<bool(float)> m_condition;

};