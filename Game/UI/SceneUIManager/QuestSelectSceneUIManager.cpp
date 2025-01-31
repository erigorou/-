#include "pch.h"
#include "Game/CommonResources.h"
#include "Game/GameResources.h"
#include "QuestSelectSceneUIManager.h"
#include "Game/UI/UserInterface.h"
#include "Game/UI/UIAnchor.h"

#include "Game/UI/Action/NormalAction.h"
#include "Game/UI/Action/TitleLogoAction.h"
#include "Game/UI/Action/TutorialButtonAction.h"
#include "Game/UI/Action/BossFightButtonAction.h"

// --------------------------------------------------------
// コンストラクタ
// --------------------------------------------------------
QuestSelectSceneUIManager::QuestSelectSceneUIManager()
{
}


// --------------------------------------------------------
// デストラクタ
// --------------------------------------------------------
QuestSelectSceneUIManager::~QuestSelectSceneUIManager()
{
}


// --------------------------------------------------------
// 初期化
// --------------------------------------------------------
void QuestSelectSceneUIManager::Initialize()
{
	// ロゴの追加
	AddUserInterface(
		"logo",
		DirectX::SimpleMath::Vector2(640.0f, -300.0f),
		DirectX::SimpleMath::Vector2(DEFAULT_SIZE),
		ANCHOR::TOP_CENTER,
		new TitleLogoAction()
	);

	// ボタンの追加
	AddUserInterface(
		"startButton",
		DirectX::SimpleMath::Vector2(640.0f, 700.0f),
		DirectX::SimpleMath::Vector2(DEFAULT_SIZE),
		ANCHOR::BOTTOM_CENTER,
		new NormalAction()
	);

	// チュートリアルボタンの追加
	AddUserInterface(
		"tutorial",
		DirectX::SimpleMath::Vector2(1580.0f, 500.0f),
		DirectX::SimpleMath::Vector2(DEFAULT_SIZE),
		ANCHOR::BOTTOM_RIGHT,
		new TutorialButtonAction()
	);

	// ボス戦ボタンの追加
	AddUserInterface(
		"bossfight",
		DirectX::SimpleMath::Vector2(1880.0f, 625.0f),
		DirectX::SimpleMath::Vector2(DEFAULT_SIZE),
		ANCHOR::BOTTOM_RIGHT,
		new BossFightButtonAction()
	);
}


// --------------------------------------------------------
// ユーザーインターフェースの追加
// --------------------------------------------------------
void QuestSelectSceneUIManager::AddUserInterface(
	const std::string textureName,
	DirectX::SimpleMath::Vector2 position,
	DirectX::SimpleMath::Vector2 scale,
	ANCHOR anchor,
	IAction* action
)
{
	auto texture = GameResources::GetInstance()->GetTexture(textureName);
	auto ui = std::make_unique<UserInterface>();

	// ユーザーインターフェースの作成
	ui->Create(texture, position, scale, anchor, action);

	// ユーザーインターフェースの追加
	m_userInterfaceList.push_back(std::move(ui));
}


// --------------------------------------------------------
// 更新
// --------------------------------------------------------
void QuestSelectSceneUIManager::Update(const float time)
{
	for (auto& ui : m_userInterfaceList)
	{
		ui->Update(time);
	}
}

// --------------------------------------------------------
// 描画
// --------------------------------------------------------
void QuestSelectSceneUIManager::Render()
{
	for (auto& ui : m_userInterfaceList)
	{
		ui->Render();
	}
}
