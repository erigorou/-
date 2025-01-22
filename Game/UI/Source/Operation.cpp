#include "pch.h"

#include "../Header/Operation.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "CommonStates.h"
#include "Game/Messenger/KeyboardMessenger.h"

// 固定値
const wchar_t* Operation::TEXT_PATH = L"Resources/Textures/UI/Operation/TEXT.png";

const wchar_t* Operation::X_PATH		= L"Resources/Textures/UI/Operation/X.png"		;
const wchar_t* Operation::SHIFT_PATH	= L"Resources/Textures/UI/Operation/Shift.png"	;
const wchar_t* Operation::UP_PATH		= L"Resources/Textures/UI/Operation/Up.png"		;
const wchar_t* Operation::DOWN_PATH		= L"Resources/Textures/UI/Operation/Down.png"	;
const wchar_t* Operation::LEFT_PATH		= L"Resources/Textures/UI/Operation/Left.png"	;
const wchar_t* Operation::RIGHT_PATH	= L"Resources/Textures/UI/Operation/Right.png"	;


// ------------------------------------
// コンストラクタ
// ------------------------------------
Operation::Operation()
	: m_pDR			(nullptr)
	, m_customShader(nullptr)
	, m_CBuffer		(nullptr)
	, m_states		(nullptr)
	, m_batch		(nullptr)
	, m_texture		()
	, m_elapsedTime	(0.0f)
	, m_totalTime	(0.0f)
{
	// デバイスリソースの取得
	m_pDR = CommonResources::GetInstance()->GetDeviceResources();
}


// ------------------------------------
// デストラクタ
// ------------------------------------
Operation::~Operation()
{
}


// ------------------------------------
// 初期化処理
// ------------------------------------
void Operation::Initialize()
{

	// 操作キーのパスを格納した配列
	const std::vector<const wchar_t*> keyPaths = 
	{
		X_PATH, SHIFT_PATH, UP_PATH, DOWN_PATH, LEFT_PATH, RIGHT_PATH
	};


	// 各キーに対して操作UIを生成し、初期化してリストに追加
	for (const auto& path : keyPaths) 
	{
		auto operateUI = std::make_unique<OperateUI>(path);
		operateUI->Initialize();
		m_operateUIs.push_back(std::move(operateUI));
	}

	// キーボードを登録
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::X,			m_operateUIs[0].get(), KeyboardMessenger::KeyPressType::DOWN);
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::LeftShift,	m_operateUIs[1].get(), KeyboardMessenger::KeyPressType::DOWN);
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::Up,			m_operateUIs[2].get(), KeyboardMessenger::KeyPressType::DOWN);
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::Down,		m_operateUIs[3].get(), KeyboardMessenger::KeyPressType::DOWN);
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::Left,		m_operateUIs[4].get(), KeyboardMessenger::KeyPressType::DOWN);
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::Right,		m_operateUIs[5].get(), KeyboardMessenger::KeyPressType::DOWN);

	// テキストUIの初期化
	m_textUI = std::make_unique<UI>(TEXT_PATH);
	m_textUI->Initialize();
}


// ------------------------------------
// 更新処理
// ------------------------------------
void Operation::Update(float elapsedTime)
{
	// 経過時間の加算
	m_totalTime += elapsedTime;
}


// ------------------------------------
// 描画処理
// ------------------------------------
void Operation::Render()
{
	for(size_t i = 0; i < m_operateUIs.size(); i++)
	{ 
		m_operateUIs[i]->Render();
	}

	// テキストUIの描画
	m_textUI->Render();
}


// ------------------------------------
// 終了処理
// ------------------------------------
void Operation::Finalize()
{
}
