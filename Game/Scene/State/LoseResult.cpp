/*
	@file	LoseResultScene.cpp
	@brief	リザルトシーンクラス
*/
#include "pch.h"
#include "LoseResult.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/Texture.h"
#include <cassert>

using namespace DirectX;
using namespace DirectX::SimpleMath;

const wchar_t* LoseResult::TEXTURE_PATH = L"Resources/Textures/UI/Lose.png";

//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
LoseResult::LoseResult()
	:
	m_spriteBatch{},
	m_texture{},
	m_texCenter{},
	m_isChangeScene{}
{
	m_commonResources = CommonResources::GetInstance();
}

//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
LoseResult::~LoseResult()
{
	// do nothing.
}

//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void LoseResult::Initialize()
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	// スプライトバッチを作成する
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

	// テクスチャの作成およびデータの取得
	mylib::Texture::LoadTexture
	(
		device,
		m_texture,
		TEXTURE_PATH
	);

	// テクスチャの大きさ
	DirectX::SimpleMath::Vector2 texSize;

	// テクスチャの中心座標を計算する
	mylib::Texture::CalculateTextureCenter
	(
		m_texture,
		texSize,
		m_texCenter
	);

	// シーン変更フラグを初期化する
	m_isChangeScene = false;
}

//---------------------------------------------------------
// 更新する
//---------------------------------------------------------
void LoseResult::Update(float elapsedTime)
{
	// 宣言をしたが、実際は使用していない変数
	UNREFERENCED_PARAMETER(elapsedTime);
}

//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void LoseResult::Render()
{
	auto states = m_commonResources->GetCommonStates();

	// スプライトバッチの開始：オプションでソートモード、ブレンドステートを指定する
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states->NonPremultiplied());

	DirectX::SimpleMath::Vector2 texPos{ 0.0f, 200.0f };

	// テクスチャを描画する
	mylib::Texture::DrawTexture
	(
		m_spriteBatch.get(),
		texPos,
		m_texture,
		m_texCenter,
		Vector2{ 1.0f, 1.0f },
		0.0f
	);

#ifdef _DEBUG
#endif // _DEBUG

	// スプライトバッチの終わり
	m_spriteBatch->End();
}

//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void LoseResult::Finalize()
{
	// do nothing.
}

//---------------------------------------------------------
// 次のシーンIDを取得する
//---------------------------------------------------------
IScene::SceneID LoseResult::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		return IScene::SceneID::TITLE;
	}

	// シーン変更がない場合
	return IScene::SceneID::NONE;
}