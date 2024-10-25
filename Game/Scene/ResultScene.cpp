/*
	@file	ResultScene.cpp
	@brief	リザルトシーンクラス
*/
#include "pch.h"
#include "ResultScene.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include <cassert>
#include "../Data/GameData.h"
#include "Libraries/MyLib/Texture.h"
#include "State/WinResult.h"
#include "State/LoseResult.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;


//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
ResultScene::ResultScene()
	: m_spriteBatch{}
	, m_texture{}
	, m_texCenter{}
	, m_isChangeScene{}
	, m_commonResources{}
	, m_winResult{}

{
	m_commonResources = CommonResources::GetInstance();
}


//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
ResultScene::~ResultScene()
{
	// do nothing.
}


//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void ResultScene::Initialize()
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	// スプライトバッチを作成する
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	// テクスチャの作成およびデータの取得
	CreateTextures();
	// オブジェクトの生成
	CreateObjects();
	// シーン変更フラグを初期化する
	m_isChangeScene = false;
}



//---------------------------------------------------------
// テクスチャ関連
//---------------------------------------------------------
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


//---------------------------------------------------------
// オブジェクトの生成
//---------------------------------------------------------
void ResultScene::CreateObjects()
{
	// 勝利リザルトの生成
	m_winResult		= std::make_unique<WinResult>	();
	m_loseResult	= std::make_unique<LoseResult>	();

	// 初期化
	m_winResult	->	Initialize();
	m_loseResult->	Initialize();

	//// ゲームデータの取得
	//m_gameData = GameData::GetInstance();

	// ゲームデータを取得し、それに応じた結果を出す
	switch (GameData::GetInstance()->GetBattleResult())
	{
	case GameData::BATTLE_RESULT::WIN:
		m_currentState = m_winResult.get();
		
		break;
	case GameData::BATTLE_RESULT::LOSE:
		m_currentState = m_loseResult.get();
		break;

	default:
		assert(false && "GameDataのVATTLE_RESULTに正しい結果が入ってません。PlaySceneのUpdateのところの判定のとこちゃんと見てん。");
		break;
	}
}


//---------------------------------------------------------
// 更新する
//---------------------------------------------------------
void ResultScene::Update(float elapsedTime)
{
	// 現在のステートを更新する
	m_currentState->Update(elapsedTime);

	// キーボードステートトラッカーを取得する
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();

	// スペースキーが押されたら
	if (kbTracker->pressed.Space)
	{
		m_isChangeScene = true;
	}
}


//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void ResultScene::Render()
{
	auto states = m_commonResources->GetCommonStates();

	// スプライトバッチの開始：オプションでソートモード、ブレンドステートを指定する
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states->NonPremultiplied());

	// TRIDENTロゴの描画位置を決める
	RECT rect{ m_commonResources->GetDeviceResources()->GetOutputSize() };
	// 画像の中心を計算する
	Vector2 pos{ rect.right / 2.0f, rect.bottom / 2.0f };


	m_spriteBatch->Draw(
		m_texture.Get(),	// テクスチャ(SRV)
		pos,				// スクリーンの表示位置(originの描画位置)
		nullptr,			// 矩形(RECT)
		Colors::White,		// 背景色
		0.0f,				// 回転角(ラジアン)
		m_texCenter,		// テクスチャの基準になる表示位置(描画中心)(origin)
		1.0f,				// スケール(scale)
		SpriteEffects_None,	// エフェクト(effects)
		0.0f				// レイヤ深度(画像のソートで必要)(layerDepth)
	);

	// スプライトバッチの終わり
	m_spriteBatch->End();


	// 勝ち負けの描画
	m_currentState->Render();

#ifdef _DEBUG
#endif // _DEBUG

}


//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void ResultScene::Finalize()
{
	// do nothing.
}


//---------------------------------------------------------
// 次のシーンIDを取得する
//---------------------------------------------------------
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