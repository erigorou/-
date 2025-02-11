// --------------------------------------------------
// 
// 名前:	ResultScene.h
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
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/Texture.h"
#include "../Sound/Sound.h"
#include "Game/UI/UIAnchor.h"
#include "Interface/IAction.h"
#include "Game/UI/Action/GoTitleButtonAction.h"
#include "Game/UI/Action/EndGameButtonAction.h"

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
	Sound::ChangeBGM(Sound::BGM_TYPE::WIN);
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
	m_uiList.push_back(std::move(ui));
}

// ---------------------------------------------
/// <summary>
/// UIを作成する
/// </summary>
// ---------------------------------------------
void ResultScene::CreateUI()
{
	// タイトルに戻るボタンの追加
	AddUserInterface(
		"GoTitleUI",
		DirectX::SimpleMath::Vector2{ RETURN_BUTTON_POSITION },
		DirectX::SimpleMath::Vector2{ BUTTON_SIZE },
		ANCHOR::TOP_LEFT,
		new GoTitleButtonAction()
	);

	// ゲーム終了ボタンの追加
	AddUserInterface(
		"GameEndUI",
		DirectX::SimpleMath::Vector2{ EXIT_BUTTON_POSITION },
		DirectX::SimpleMath::Vector2{ BUTTON_SIZE },
		ANCHOR::TOP_LEFT,
		new EndGameButtonAction()
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
	// キーボードステートトラッカーを取得する
	const auto& kbTracker = CommonResources::GetInstance()->GetInputManager()->GetKeyboardTracker();

	// スペースキーが押されたら
	if (kbTracker->pressed.Space){
		m_isChangeScene = true;
	}

	// UIの更新
	for (auto& ui : m_uiList)
	{
		ui->Update(elapsedTime);
	}
}

// ---------------------------------------------
/// <summary>
/// 描画する
/// </summary>
// ---------------------------------------------
void ResultScene::Render()
{
	// コモンステートを取得する
	auto states = CommonResources::GetInstance()->GetCommonStates();

	// スプライトバッチの開始：オプションでソートモード、ブレンドステートを指定する
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, states->NonPremultiplied());
	// 背景の描画
	DrawBackground();
	// スクリーンショットの描画
	DrawCaptureTexture();
	// スプライトバッチの終わり
	m_spriteBatch->End();

	// UIの描画
	for (auto& ui : m_uiList)
	{
		ui->Render();
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
/// 背景を描画する
/// </summary>
// ---------------------------------------------
void ResultScene::DrawBackground()
{
	m_spriteBatch->Draw(
		m_texture.Get(),			// テクスチャ(SRV)
		BACKGROUND_POSITION,		// スクリーンの表示位置(originの描画位置)
		nullptr,					// 矩形(RECT)
		DirectX::Colors::White,		// 背景色
		BACKGROUND_ROTATION,		// 回転角(ラジアン)
		m_texCenter,				// テクスチャの基準になる表示位置(描画中心)(origin)
		BACKGROUND_SCALE,			// スケール(scale)
		DirectX::SpriteEffects_None,// エフェクト(effects)
		0.0f						// レイヤ深度(画像のソートで必要)(layerDepth)
	);
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