// --------------------------------------------------
// 
// 名前:	SelectScene.cpp
// 内容:	セレクトシーンを描画するクラス
// 作成:	池田桜輔
// 
// --------------------------------------------------
// インクルード
#include "pch.h"
#include "SelectScene.h"
#include "Game/CommonResources.h"
#include "Game/GameResources.h"
#include "DeviceResources.h"
#include "Game/UI/UserInterface.h"
#include "Libraries/MyLib/Texture.h"
#include "../Sound/Sound.h"
#include "Game/UI/UIAnchor.h"
#include "Interface/IAction.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/Messenger/KeyboardMessenger.h"

// ----------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
//-----------------------------------------------
SelectScene::SelectScene()
	: 
	m_spriteBatch{},
	m_isChangeScene{},
	m_gameData{}
{
}

// ----------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ----------------------------------------------
SelectScene::~SelectScene()
{
	Finalize();
}

// ----------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// ----------------------------------------------
void SelectScene::Initialize()
{
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	// スプライトバッチを作成する
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	// ゲームデータを取得する
	m_gameData = GameData::GetInstance();
	// キーボードの登録
	RegisterKeyboard();
}

// ----------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
// -----------------------------------------------
void SelectScene::Update(float elapsedTime)
{
	// UIの更新
	for (auto& ui : m_uiList)
	{
		ui->Update(elapsedTime);
	}
}

// ----------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
// ----------------------------------------------
void SelectScene::Render()
{
	// コモンステートを取得する
	auto states = CommonResources::GetInstance()->GetCommonStates();
	// スプライトバッチの開始：オプションでソートモード、ブレンドステートを指定する
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, states->NonPremultiplied());
	// スプライトバッチの終わり
	m_spriteBatch->End();
	// UIの描画
	for (auto& ui : m_uiList)
	{
		ui->Render();
	}
}

// ----------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
// ----------------------------------------------
void SelectScene::Finalize()
{
	// UIの削除
	m_uiList.clear();
	// スプライトバッチの削除
	m_spriteBatch.reset();
	// キーボードの登録解除
	KeyboardMessenger::Clear();
}

// -----------------------------------------------
/// <summary>
/// 次のシーンIDを取得する
/// </summary>
/// <returns>次のシーンID</returns>
// -----------------------------------------------
IScene::SceneID SelectScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		return IScene::SceneID::PLAY;
	}
	// シーン変更がない場合
	return IScene::SceneID::NONE;
}

// -----------------------------------------------
/// <summary>
/// Escapeが押された場合のシーンIDを取得する
/// </summary>
/// <returns>変更後のシーンID</returns>
// ---------------------------------
IScene::SceneID SelectScene::GetPrevSceneID() const
{
	// シーン変更がない場合
	return IScene::SceneID::TITLE;
}


// ----------------------------------------------
/// <summary>
/// 使用するキーボードを登録する
/// </summary>
// -----------------------------------------------
void SelectScene::RegisterKeyboard()
{
	// キー入力の登録
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::Up, this, KeyboardMessenger::KeyPressType::PRESSED);
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::Down, this, KeyboardMessenger::KeyPressType::PRESSED);
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::Space, this, KeyboardMessenger::KeyPressType::PRESSED);
}


// ----------------------------------------------
/// <summary>
/// キー入力（押された瞬間）
/// </summary>
/// <param name="key">キー</param>
// -----------------------------------------------
void SelectScene::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	// スペースキーが押されたら
	if (key == DirectX::Keyboard::Keys::Space){
		OnKeyDownSpace();
	}
	// 上キーが押されたら
	if (key == DirectX::Keyboard::Keys::Up){
		OnKeyDownArrowUp();
	}
	// 下キーが押されたら
	if (key == DirectX::Keyboard::Keys::Down){
		OnKeyDownArrowDown();
	}
}

// ----------------------------------------------
/// <summary>
/// キー入力（押されている間）
/// </summary>
/// <param name="key">キー</param>
// -----------------------------------------------
void SelectScene::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}

// ----------------------------------------------
/// <summary>
// 上キーが押されたときの処理
/// </summary>
// -----------------------------------------------
void SelectScene::OnKeyDownArrowUp()
{
	// 上キーが押されたときの処理
}

// ----------------------------------------------
/// <summary>
// 下キーが押されたときの処理
/// </summary>
// -----------------------------------------------
void SelectScene::OnKeyDownArrowDown()
{
	// 下キーが押されたときの処理
}

// ----------------------------------------------
/// <summary>
// スペースキーが押されたときの処理
/// </summary>
// -----------------------------------------------
void SelectScene::OnKeyDownSpace()
{
	// スペースキーが押されたときの処理
	m_isChangeScene = true;
}

