// ----------------------------------------------------------------------------------
// 名前:    BehaviorNodeLoader.cpp
// 説明:    Jsonで記録したBTのノード群を解析・生成する
// 作成:    池田桜輔
// 日付:    2025/06/06
// ----------------------------------------------------------------------------------

#include "pch.h"
#include "BehaviorNodeLoader.h"
#include <fstream>

using json = nlohmann::json;

/// <summary>
/// Jsonファイルからノードをロードする関数
/// </summary>
/// <param name="filePath">Jsonファイルのパス</param>
/// <param name="actionMap">アクションマップ</param>
/// <param name="conditionMap">条件マップ</param>
std::unique_ptr<ICompositeNode> BehaviorNodeLoader::LoadFromFile(const std::string& filePath, const ActionMap& actionMap, const ConditionMap& conditionMap)
{
	// Jsonファイルを開く
	std::ifstream ifs(filePath);
	if (!ifs) {
		MessageBoxA(nullptr, ("Jsonファイルが開けません: " + filePath).c_str(), "BehaviorNodeLoader", MB_OK);
		return nullptr;
	}

	// Jsonデータをパース
	json nodeJson;
	try {
		ifs >> nodeJson;
	}
	catch (const json::parse_error& e) {
		MessageBoxA(nullptr, ("Jsonのパースエラー: " + std::string(e.what())).c_str(), "BehaviorNodeLoader", MB_OK);
		return nullptr;
	}

    // ノードを生成（IBehaviorNodeとして一旦受ける）
    auto node = CreateNodeFromJson(nodeJson, actionMap, conditionMap);

    // ICompositeNode*にキャストして確認
    ICompositeNode* composite = dynamic_cast<ICompositeNode*>(node.get());
    if (!composite) {
        MessageBoxA(nullptr, "ルートノードはICompositeNodeである必要があります。", "BehaviorNodeLoader", MB_OK);
        return nullptr;
    }

    // 所有権を移す（キャストして解放）
    return std::unique_ptr<ICompositeNode>(static_cast<ICompositeNode*>(node.release()));
}

/// <summary>
/// Jsonデータからノードを生成する関数
/// </summary>
/// <param name="nodeJson">ノード定義のJsonオブジェクト</param>
/// <param name="actionMap">アクションマップ</param>
/// <param name="conditionMap">条件マップ</param>
/// <returns>生成されたノード</returns>
std::unique_ptr<IBehaviorNode> BehaviorNodeLoader::CreateNodeFromJson(
    const json& nodeJson,
    const ActionMap& actionMap,
    const ConditionMap& conditionMap)
{
    const std::string type = nodeJson["type"];

    // セレクタータイプ：複数の子ノードから最初に成功したものを返す
    if (type == "Selector") {
        auto node = std::make_unique<SelectorNode>();
        for (const auto& child : nodeJson["children"]) {
            node->AddNode(CreateNodeFromJson(child, actionMap, conditionMap));
        }
        return node;
    }

    // シーケンスタイプ：すべての子ノードを順に実行し、すべて成功でSUCCESS
    if (type == "Sequence") {
        auto node = std::make_unique<SequenceNode>();
        for (const auto& child : nodeJson["children"]) {
            node->AddNode(CreateNodeFromJson(child, actionMap, conditionMap));
        }
        return node;
    }

    // デコレータタイプ：条件を満たすときだけ子ノードを実行する
    if (type == "Decorator") {
        const std::string condKey = nodeJson["condition"];
        auto it = conditionMap.find(condKey);
        if (it == conditionMap.end()) {
            MessageBoxA(nullptr, ("条件未登録: " + condKey).c_str(), "BehaviorNodeLoader", MB_OK);
            return nullptr;
        }
        auto node = std::make_unique<DecoratorNode>(it->second);
        node->AddNode(CreateNodeFromJson(nodeJson["child"], actionMap, conditionMap));
        return node;
    }

    // アクションタイプ：登録された処理を実行するノード
    if (type == "Action") {
        const std::string actKey = nodeJson["action"];
        auto it = actionMap.find(actKey);
        if (it == actionMap.end()) {
            MessageBoxA(nullptr, ("アクション未登録: " + actKey).c_str(), "BehaviorNodeLoader", MB_OK);
            return nullptr;
        }
        return std::make_unique<ActionNode>(it->second);
    }

    // 不明なタイプに対するエラーハンドリング
    MessageBoxA(nullptr, ("不明なノードタイプ: " + type).c_str(), "BehaviorNodeLoader", MB_OK);
    return nullptr;
}