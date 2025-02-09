// --------------------------------------------
// 
// 名前:	Operation.cpp
// 機能:	操作説明UIのクラス
//			マネージャー的な役割を持つ
// 製作:	池田桜輔
// 
// --------------------------------------------
// インクルード
#include "pch.h"
#include "Operation.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "CommonStates.h"
#include "Game/Messenger/KeyboardMessenger.h"


// ---------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// ---------------------------------------------------------
Operation::Operation()
	:
	m_customShader(nullptr),
	m_CBuffer(nullptr),
	m_states(nullptr),
	m_batch(nullptr),
	m_texture{},
	m_elapsedTime{},
	m_totalTime{}
{
}

// ---------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------------------------
Operation::~Operation()
{
}

// ---------------------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// ---------------------------------------------------------
void Operation::Initialize()
{

	// 各キーに対して操作UIを生成し、初期化してリストに追加
	for (const auto& key : keys)
	{
		auto operateUI = std::make_unique<OperateUI>(key);
		operateUI->Initialize();
		m_operateUIs.push_back(std::move(operateUI));
	}

	// キーボードイベントに使用キーを登録
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::X, m_operateUIs[0].get(), KeyboardMessenger::KeyPressType::DOWN);
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::LeftShift, m_operateUIs[1].get(), KeyboardMessenger::KeyPressType::DOWN);
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::Up, m_operateUIs[2].get(), KeyboardMessenger::KeyPressType::DOWN);
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::Down, m_operateUIs[3].get(), KeyboardMessenger::KeyPressType::DOWN);
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::Left, m_operateUIs[4].get(), KeyboardMessenger::KeyPressType::DOWN);
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::Right, m_operateUIs[5].get(), KeyboardMessenger::KeyPressType::DOWN);

	// テキストUIの初期化
	m_textUI = std::make_unique<UI>(TEXT_KEY[0]);
	m_textUI->Initialize();
}

// ---------------------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------------------
void Operation::Update(float elapsedTime)
{
	// 経過時間の加算
	m_totalTime += elapsedTime;
}

// ---------------------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
// ---------------------------------------------------------
void Operation::Render()
{
	for (size_t i = 0; i < m_operateUIs.size(); i++)
	{
		m_operateUIs[i]->Render();
	}

	// テキストUIの描画
	m_textUI->Render();
}

// ---------------------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
// ---------------------------------------------------------
void Operation::Finalize()
{
}