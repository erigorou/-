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
// 	定数
// ------------------------------
public:
	// デフォルトの大きさ
	static constexpr DirectX::SimpleMath::Vector2 DEFAULT_SIZE = DirectX::SimpleMath::Vector2(1.0f, 1.0f);
	// ロゴの初期値
	static constexpr DirectX::SimpleMath::Vector2 LOGO_POSITION = DirectX::SimpleMath::Vector2(640.0f, -300.0f);
	// ボタンの初期値
	static constexpr DirectX::SimpleMath::Vector2 BUTTON_POSITION = DirectX::SimpleMath::Vector2(640.0f, 700.0f);
	// チュートリアルボタンの初期値
	static constexpr DirectX::SimpleMath::Vector2 TUTORIAL_BUTTON_POSITION = DirectX::SimpleMath::Vector2(1580.0f, 500.0f);
	// ボス戦ボタンの初期値
	static constexpr DirectX::SimpleMath::Vector2 BOSS_FIGHT_BUTTON_POSITION = DirectX::SimpleMath::Vector2(1880.0f, 625.0f);


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
// メンバ変数
// ------------------------------
private:
	// ユーザーインターフェースリスト
	std::vector<std::unique_ptr<UserInterface>> m_userInterfaceList;
};


