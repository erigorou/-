// --------------------------------------------------
// 
// 名前:	ResultScene.cpp
// 内容:	リザルトシーンを描画するクラス
// 作成:	池田桜輔
// 
// --------------------------------------------------
// インクルード
#include "pch.h"
#include "ResultScene.h"
#include "Game/CommonResources.h"
#include "Game/GameResources.h"
#include "DeviceResources.h"
#include "Game/UI/UserInterface.h"
#include "Game/UI/ScreenShot/ScreenShotUserInterface.h"
#include "Libraries/MyLib/Texture.h"
#include "../Sound/Sound.h"
#include "Game/UI/UIAnchor.h"
#include "Interface/IAction.h"
#include "Game/UI/Action/GoTitleButtonAction.h"
#include "Game/UI/Action/EndGameButtonAction.h"
#include "Game/Messenger/KeyboardMessenger.h"

// ---------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// ---------------------------------------------
ResultScene::ResultScene()
	: 
	m_spriteBatch{},
	m_texture{},
	m_texCenter{},
	m_isChangeScene{},
	m_gameData{},
	m_result{}
{
	// スクリーンショットを取得
	m_captureTexture = GameData::GetInstance()->GetScreenShot();
}

// ---------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------------
ResultScene::~ResultScene()
{
}

// ---------------------------------------------
/// <summary>
/// 初期化する
/// </summary>
// ---------------------------------------------
void ResultScene::Initialize()
{
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// スプライトバッチを作成する
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	// テクスチャの作成およびデータの取得
	CreateTextures();
	// シーン変更フラグを初期化する
	m_isChangeScene = false;
	// UIの作成
	CreateUI();

	// BGMの再生
	Sound::ChangeBGM("WinBGM");

	// キーの登録
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::Space, this, KeyboardMessenger::KeyPressType::PRESSED);
}

// ---------------------------------------------
/// <summary>
/// テクスチャを作成し、データを取得する
/// </summary>
// ---------------------------------------------
void ResultScene::CreateTextures()
{
	// テクスチャの作成
	m_texture = GameResources::GetInstance()->GetTexture("result");

	DirectX::SimpleMath::Vector2 texSize{};

	// テクスチャのサイズと中心点を計算する
	mylib::Texture::CalculateTextureCenter
	(
		m_texture,
		texSize,
		m_texCenter
	);

	// スクリーンショットのサイズと中心点を計算する
	mylib::Texture::CalculateTextureCenter
	(
		m_captureTexture,
		texSize,
		m_captureTexCenter
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
void ResultScene::AddUserInterface(
	const std::string textureName,
	DirectX::SimpleMath::Vector2 position,
	DirectX::SimpleMath::Vector2 scale,
	ANCHOR anchor,
	IAction* action,
	UINT layer)
{
	// テクスチャの取得
	ID3D11ShaderResourceView* texture = nullptr;
	texture = GameResources::GetInstance()->GetTexture(textureName);

	// ユーザーインターフェースの生成
	auto ui = std::make_unique<UserInterface>();
	// ユーザーインターフェースの作成
	ui->Create(texture, position, scale, anchor, action);
	// レイヤーの設定
	ui->SetLayer(layer);
	// ユーザーインターフェースの登録
	m_uiList.push_back(std::move(ui));
}

// ---------------------------------------------
/// <summary>
/// UIを作成する
/// </summary>
// ---------------------------------------------
void ResultScene::CreateUI()
{
	// 背景の追加
	AddUserInterface(
		"result",
		DirectX::SimpleMath::Vector2{ BACKGROUND_POSITION },
		DirectX::SimpleMath::Vector2{ BACKGROUND_SCALE },
		ANCHOR::MIDDLE_CENTER,
		nullptr,
		300
	);

	// タイトルに戻るボタンの追加
	AddUserInterface(
		"GoTitleUI",
		DirectX::SimpleMath::Vector2{ RETURN_BUTTON_POSITION },
		DirectX::SimpleMath::Vector2{ BUTTON_SIZE },
		ANCHOR::TOP_LEFT,
		new GoTitleButtonAction(),
		310
	);

	// ゲーム終了ボタンの追加
	AddUserInterface(
		"GameEndUI",
		DirectX::SimpleMath::Vector2{ EXIT_BUTTON_POSITION },
		DirectX::SimpleMath::Vector2{ BUTTON_SIZE },
		ANCHOR::TOP_LEFT,
		new EndGameButtonAction(),
		320
	);

	// スクリーンショットのUIを作成
	m_screenShotUI = std::make_unique<ScreenShotUserInterface>();
	m_screenShotUI->Create(
		CAPTURE_POSITION,								// 位置
		CAPTURE_ROTATION,								// 回転
		m_captureTexCenter,								// 中心
		DirectX::SimpleMath::Vector2{ CAPTURE_SCALE }	// スケール
	);
}

// ---------------------------------------------
/// <summary>
/// シーンを更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------
void ResultScene::Update(float elapsedTime)
{
	// UIの更新
	for (auto& ui : m_uiList)
	{
		ui->Update(elapsedTime);
	}
}

// ---------------------------------------------
/// <summary>
/// スクリーンショットを描画する
/// </summary>
// ---------------------------------------------
void ResultScene::DrawCaptureTexture()
{
	// 背景前面にゲームデータがもつスクショを描画
	if (GameData::GetInstance()->GetScreenShot())
	{
		m_spriteBatch->Draw(
			m_captureTexture.Get(),		// テクスチャ(SRV)
			CAPTURE_POSITION,			// スクリーンの表示位置(originの描画位置)
			nullptr,					// 矩形(RECT)
			DirectX::Colors::White,		// 背景色
			CAPTURE_ROTATION,			// 回転角(ラジアン)
			m_captureTexCenter,			// テクスチャの基準になる表示位置(描画中心)(origin)
			CAPTURE_SCALE,				// スケール(scale)
			DirectX::SpriteEffects_None,// エフェクト(effects)
			0.0f						// レイヤ深度(画像のソートで必要)(layerDepth)
		);
	}
}


// ---------------------------------------------
/// <summary>
/// キーボードが押された瞬間の処理
/// </summary>
/// <param name="key">キー</param>
// ---------------------------------------------
void ResultScene::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	// スペースキーが押されたら
	if (key == DirectX::Keyboard::Keys::Space)
	{
		m_isChangeScene = true;
	}
}


// ---------------------------------------------
/// <summary>
/// キーボードが押されている間の処理
/// </summary>
/// <param name="key">キー</param>
// ---------------------------------------------
void ResultScene::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}



// ---------------------------------------------
/// <summary>
/// 後始末する
/// </summary>
// ---------------------------------------------
void ResultScene::Finalize()
{
}

// ---------------------------------------------
/// <summary>
/// 次のシーンIDを取得する
/// </summary>
/// <returns>次のシーンID</returns>
// ---------------------------------------------
IScene::SceneID ResultScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		return IScene::SceneID::TITLE;
	}

	// シーン変更がない場合
	return IScene::SceneID::NONE;
}

// ---------------------------------------------
/// <summary>
/// Escapeが押されたときのシーンIDを取得する
/// </summary>
/// <returns>次のシーンID</returns>
// ---------------------------------------------
IScene::SceneID ResultScene::GetPrevSceneID() const
{
	return IScene::SceneID::TITLE;
}
