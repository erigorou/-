#pragma once
#include "pch.h"
#include "Game/UI/UIAnchor.h"
#include "Interface/IAction.h"

class NormalAction;
class TitleLogoAction;
class UserInterface;


class QuestSelectSceneUIManager
{
// ------------------------------
// 公開関数
// ------------------------------
public:
	// コンストラクタ
	QuestSelectSceneUIManager();
	// デストラクタ
	~QuestSelectSceneUIManager();
	// 初期化
	void Initialize();
	// 更新
	void Update(const float time);
	// 描画
	void Render();


// ------------------------------
// 内部関数
// ------------------------------
private:
	// ユーザーインターフェースの追加
	void AddUserInterface(
		const std::string textureName,
		DirectX::SimpleMath::Vector2 position,
		DirectX::SimpleMath::Vector2 scale,
		ANCHOR anchor,
		IAction* action
	);

// ------------------------------
// 内部変数
// ------------------------------
private:
	// ユーザーインターフェースリスト
	std::vector<std::unique_ptr<UserInterface>> m_userInterfaceList;
};


