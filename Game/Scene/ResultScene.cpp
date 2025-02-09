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
#include "DeviceResources.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/Texture.h"
#include "../Sound/Sound.h"
#include "State/WinResult.h"
#include "State/LoseResult.h"

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
	, m_winResult{}

{
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
	// オブジェクトの生成
	CreateObjects();
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

	// テクスチャを読み込む
	mylib::Texture::LoadTexture(
		device,
		m_texture,
		L"Resources/Textures/SPACEでタイトル.png"
	);

	DirectX::SimpleMath::Vector2 texSize{};

	// テクスチャのサイズと中心点を計算する
	mylib::Texture::CalculateTextureCenter
	(
		m_texture,
		texSize,
		m_texCenter
	);
}

// ---------------------------------------------
/// <summary>
/// オブジェクトを生成する
/// </summary>
// ---------------------------------------------
void ResultScene::CreateObjects()
{
	// 勝利リザルトの生成
	m_winResult = std::make_unique<WinResult>();
	m_loseResult = std::make_unique<LoseResult>();

	// 初期化
	m_winResult->Initialize();
	m_loseResult->Initialize();

	// ゲームデータを取得し、それに応じた結果を出す
	switch (GameData::GetInstance()->GetBattleResult())
	{
		// 勝利
	case GameData::BATTLE_RESULT::WIN:
		m_currentState = m_winResult.get();
		Sound::ChangeBGM(Sound::BGM_TYPE::WIN);
		break;

		// 敗北
	case GameData::BATTLE_RESULT::LOSE:
		m_currentState = m_loseResult.get();
		Sound::ChangeBGM(Sound::BGM_TYPE::LOSE);
		break;

		// それ以外
	default:
		assert(false && "GameDataのVATTLE_RESULTに正しい結果が入ってません。");
		break;
	}
}

// ---------------------------------------------
/// <summary>
/// シーンを更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------
void ResultScene::Update(float elapsedTime)
{
	// 現在のステートを更新する
	m_currentState->Update(elapsedTime);

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

	// TRIDENTロゴの描画位置を決める
	RECT rect{ CommonResources::GetInstance()->GetDeviceResources()->GetOutputSize() };
	// 画像の中心を計算する
	DirectX::SimpleMath::Vector2 pos{ rect.right / 2.0f, rect.bottom / 2.0f };

	m_spriteBatch->Draw(
		m_texture.Get(),			// テクスチャ(SRV)
		pos,						// スクリーンの表示位置(originの描画位置)
		nullptr,					// 矩形(RECT)
		DirectX::Colors::White,		// 背景色
		0.0f,						// 回転角(ラジアン)
		m_texCenter,				// テクスチャの基準になる表示位置(描画中心)(origin)
		1.0f,						// スケール(scale)
		DirectX::SpriteEffects_None,// エフェクト(effects)
		0.0f						// レイヤ深度(画像のソートで必要)(layerDepth)
	);

	// スプライトバッチの終わり
	m_spriteBatch->End();

	// 勝ち負けの描画
	m_currentState->Render();
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