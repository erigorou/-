// ----------------------------------------------------------------------------------
//
// 名前:	BehaviorNodeLoader.h
// 説明:    Jsonで記録したBTのノード群を取得するクラス
// 作成:	池田桜輔
// 日付:    2025/06/06
//
// ----------------------------------------------------------------------------------

#pragma once
#include "pch.h"
#include <nlohmann/json.hpp>

#include "Libraries/MyLib/BehaviorTree/Interface/IBehaviorNode.h"
#include "Libraries/MyLib/BehaviorTree/Interface/IcompositeNode.h"
#include "Libraries/MyLib/BehaviorTree/Node/RootNode/Root.h"
#include "Libraries/MyLib/BehaviorTree/Node/SelectorNode/SelectorNode.h"
#include "Libraries/MyLib/BehaviorTree/Node/SequenceNode/SequenceNode.h"
#include "Libraries/MyLib/BehaviorTree/Node/DecoratorNode/DecoratorNode.h"
#include "Libraries/MyLib/BehaviorTree/Node/ActionNode/ActionNode.h"


/// <summary>
/// Jsonで記録したBTのノード群を取得するクラス
/// </summary>
class BehaviorNodeLoader
{
	// -----------------------
	// 固定値
	// -----------------------
public:
	// ノードのアクションを格納するマップ
	using ActionMap = std::unordered_map<std::string, std::function<IBehaviorNode::NodeState()>>;
	// ノードの条件を格納するマップ
	using ConditionMap = std::unordered_map<std::string, std::function<bool()>>;


	//------------------------
	// アクセサ
	// -----------------------
public:
	// Jsonファイルからノードをロード
	static std::unique_ptr<ICompositeNode> LoadFromFile(
		const std::string& filePath,
		const ActionMap& actionMap,
		const ConditionMap& conditionMap
	);

	//------------------------
	// メンバ関数(公開)
	// -----------------------
public:
	// コンストラクタ
	BehaviorNodeLoader();
	// デストラクタ
	~BehaviorNodeLoader() = default;

	// -----------------------
	// メンバ関数(非公開)
	// -----------------------
private:
	// Jsonファイルからノードを作成
	static std::unique_ptr<IBehaviorNode> CreateNodeFromJson(
		const nlohmann::json& json,
		const ActionMap& actionMap,
		const ConditionMap& conditionMap
	);

};