// -------------------------------------------------------------------
// 
// 名前:	QuestSelectSceneUIManager.cpp
// 機能:	タイトルシーンでのUI管理クラス	
//			チュートリアルかボスを選択するボタンを表示する
// 製作:	池田桜輔
// 
// -------------------------------------------------------------------
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

// ---------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// ---------------------------------------------------------

QuestSelectSceneUIManager::QuestSelectSceneUIManager()
{
}

// ---------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------------------------
QuestSelectSceneUIManager::~QuestSelectSceneUIManager()
{
}

// ---------------------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// ---------------------------------------------------------
void QuestSelectSceneUIManager::Initialize()
{
	// ロゴの追加
	AddUserInterface(
		"logo",
		DirectX::SimpleMath::Vector2(LOGO_POSITION),
		DirectX::SimpleMath::Vector2(DEFAULT_SIZE),
		ANCHOR::TOP_CENTER,
		new TitleLogoAction()
	);

	// ボタンの追加
	AddUserInterface(
		"startButton",
		DirectX::SimpleMath::Vector2(BUTTON_POSITION),
		DirectX::SimpleMath::Vector2(DEFAULT_SIZE),
		ANCHOR::BOTTOM_CENTER,
		new NormalAction()
	);

	// チュートリアルボタンの追加
	AddUserInterface(
		"tutorial",
		DirectX::SimpleMath::Vector2(TUTORIAL_BUTTON_POSITION),
		DirectX::SimpleMath::Vector2(DEFAULT_SIZE),
		ANCHOR::BOTTOM_RIGHT,
		new TutorialButtonAction()
	);

	// ボス戦ボタンの追加
	AddUserInterface(
		"bossfight",
		DirectX::SimpleMath::Vector2(BOSS_FIGHT_BUTTON_POSITION),
		DirectX::SimpleMath::Vector2(DEFAULT_SIZE),
		ANCHOR::BOTTOM_RIGHT,
		new BossFightButtonAction()
	);
}

// ---------------------------------------------------------
/// <summary>
/// ユーザーインターフェースの追加
/// </summary>
/// <param name="textureName">テクスチャ名</param>
/// <param name="position">位置</param>
/// <param name="scale">スケール</param>
/// <param name="anchor">アンカー</param>
/// <param name="action">アクション</param>
// ---------------------------------------------------------
void QuestSelectSceneUIManager::AddUserInterface(
	const std::string textureName,
	DirectX::SimpleMath::Vector2 position,
	DirectX::SimpleMath::Vector2 scale,
	ANCHOR anchor,
	IAction* action
)
{
	// テクスチャの取得
	auto texture = GameResources::GetInstance()->GetTexture(textureName);
	// ユーザーインターフェースの生成
	auto ui = std::make_unique<UserInterface>();
	// ユーザーインターフェースの作成
	ui->Create(texture, position, scale, anchor, action);
	// ユーザーインターフェースの登録
	m_userInterfaceList.push_back(std::move(ui));
}

// ---------------------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="time">経過時間</param>
// ---------------------------------------------------------
void QuestSelectSceneUIManager::Update(const float time)
{
	for (auto& ui : m_userInterfaceList)
	{
		ui->Update(time);
	}
}

// ---------------------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
// ---------------------------------------------------------

void QuestSelectSceneUIManager::Render()
{
	for (auto& ui : m_userInterfaceList)
	{
		ui->Render();
	}
}