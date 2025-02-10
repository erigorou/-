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
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/Texture.h"
#include "../Sound/Sound.h"

// ---------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// ---------------------------------------------
ResultScene::ResultScene()
	: m_spriteBatch{}
	, m_texture{}
	, m_texCenter{}
	, m_isChangeScene{}
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
}

// ---------------------------------------------
/// <summary>
/// テクスチャを作成し、データを取得する
/// </summary>
// ---------------------------------------------
void ResultScene::CreateTextures()
{
	// デバイスの取得
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

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
	if (kbTracker->pressed.Space)
	{
		m_isChangeScene = true;
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
	
	RECT rect{ CommonResources::GetInstance()->GetDeviceResources()->GetOutputSize() };
	// スクリーンの中心を計算する
	DirectX::SimpleMath::Vector2 pos{ rect.right / 2.0f, rect.bottom / 2.0f };
	DirectX::SimpleMath::Vector2 pos2{ 350.0f , rect.bottom / 2.0f };


	m_spriteBatch->Draw(
		m_texture.Get(),			// テクスチャ(SRV)
		pos,						// スクリーンの表示位置(originの描画位置)
		nullptr,					// 矩形(RECT)
		DirectX::Colors::White,		// 背景色
		0.0f,// 回転角(ラジアン)
		m_texCenter,				// テクスチャの基準になる表示位置(描画中心)(origin)
		1.0f,						// スケール(scale)
		DirectX::SpriteEffects_None,// エフェクト(effects)
		0.0f						// レイヤ深度(画像のソートで必要)(layerDepth)
	);

	// 背景前面にゲームデータがもつスクショを描画
	if (GameData::GetInstance()->GetScreenShot())
	{
		m_spriteBatch->Draw(
			m_captureTexture.Get(),
			pos2,
			nullptr,
			DirectX::Colors::White,
			DirectX::XMConvertToRadians(-20.0f),// 回転角(ラジアン)
			m_captureTexCenter,
			0.5f,
			DirectX::SpriteEffects_None,
			0.0f
		);
	}

	// スプライトバッチの終わり
	m_spriteBatch->End();
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