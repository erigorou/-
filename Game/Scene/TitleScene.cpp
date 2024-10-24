/*
	@file	TitleScene.cpp
	@brief	タイトルシーンクラス
*/
#include "pch.h"
#include "TitleScene.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/DebugString.h"
#include "../Factory/Factory.h"
#include "../Camera/Camera.h"
#include "../Stage/Floor/Floor.h"
#include <cassert>

using namespace DirectX;
using namespace DirectX::SimpleMath;

//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
TitleScene::TitleScene()
	:
	m_spriteBatch{},
	m_spriteFont{},
	m_texture{},
	m_texCenter{},
	m_isChangeScene{}
{
	m_commonResources = CommonResources::GetInstance();
}

//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
TitleScene::~TitleScene()
{
	// do nothing.
}

//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void TitleScene::Initialize()
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	// スプライトバッチを作成する
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

	// スプライトフォントを作成する
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(
		device,
		L"Resources/Fonts/SegoeUI_18.spritefont"
	);

	// テクスチャをロードする
	LoadTextures();

	// テクスチャの中心座標を計算する
	CalculateTextureCenter();

	// オブジェクトを生成する
	CreateObjects();

	// シーン変更フラグを初期化する
	m_isChangeScene = false;
}


//---------------------------------------------------------
// ロードする
//---------------------------------------------------------
void TitleScene::LoadTextures()
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();

	// 画像をロードする
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,
			L"Resources/Textures/LOGO.png",
			nullptr,
			m_texture.ReleaseAndGetAddressOf()
		)
	);

	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,
			L"Resources/Textures/SPACEでスタート.png",
			nullptr,
			m_texture2.ReleaseAndGetAddressOf()
		)
	);
}


//---------------------------------------------------------
// テクスチャの中心を取得する
//---------------------------------------------------------
void TitleScene::CalculateTextureCenter()
{
	// 一時的な変数の宣言
	Microsoft::WRL::ComPtr<ID3D11Resource> resource{};
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D{};
	D3D11_TEXTURE2D_DESC desc{};
	Vector2 texSize{};

	// LOGO.png の中心位置を計算
	m_texture->GetResource(resource.GetAddressOf());
	resource.As(&tex2D);
	tex2D->GetDesc(&desc);
	texSize.x = static_cast<float>(desc.Width);
	texSize.y = static_cast<float>(desc.Height);
	m_texCenter1 = texSize / 2.0f;

	// SPACEでスタート.png の中心位置を計算
	resource = nullptr;
	tex2D = nullptr;
	desc = {};

	m_texture2->GetResource(resource.GetAddressOf());
	resource.As(&tex2D);
	tex2D->GetDesc(&desc);
	texSize.x = static_cast<float>(desc.Width);
	texSize.y = static_cast<float>(desc.Height);
	m_texCenter2 = texSize / 2.0f;

	RECT rect{ m_commonResources->GetDeviceResources()->GetOutputSize() };
	// 射影行列を作成する
	m_projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 100000.0f
	);
}


//---------------------------------------------------------
// オブジェクトの生成
//---------------------------------------------------------
void TitleScene::CreateObjects()
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();

	m_camera	= Factory::CreateCamera	();
	m_floor		= Factory::CreateFloor	(device);



	// タイトルシーンのカメラステートを設定
	m_camera->ChangeState(m_camera->GetTitleState());
}


//---------------------------------------------------------
// 更新する
//---------------------------------------------------------
void TitleScene::Update(float elapsedTime)
{
	// 宣言をしたが、実際は使用していない変数
	UNREFERENCED_PARAMETER(elapsedTime);

	// キーボードステートトラッカーを取得する
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();

	DirectX::SimpleMath::Vector3 zeroV = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Matrix zeroM = DirectX::SimpleMath::Matrix::Identity;

	m_camera->Update(zeroV, zeroV, zeroM, elapsedTime);

	// スペースキーが押されたら
	if (kbTracker->pressed.Space)
	{
		m_isChangeScene = true;
	}
}

//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void TitleScene::Render()
{
	auto context	= m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states		= m_commonResources	->	GetCommonStates		();
	auto view		= m_camera			->	GetViewMatrix		();


	// 床の描画
	m_floor->Render(context, view, m_projection);

	// スプライトバッチの開始：オプションでソートモード、ブレンドステートを指定する
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states->NonPremultiplied());

	DrawTexture();

	// スプライトバッチの終わり
	m_spriteBatch->End();

#ifdef _DEBUG
#endif // _DEBUG
}


//---------------------------------------------------------
// テクスチャの描画
//---------------------------------------------------------
void TitleScene::DrawTexture()
{
	// TRIDENTロゴの描画位置を決める
	RECT rect{ m_commonResources->GetDeviceResources()->GetOutputSize() };

	// 画像の中心を計算する
	Vector2 pos{ rect.right / 2.0f, rect.bottom / 2.0f };
	Vector2 titlePos = pos + TITLE_DIRECTION_CENTER;
	

	// LOGO.png を中央に描画する
	m_spriteBatch->Draw(
		m_texture.Get(),	// テクスチャ(SRV)
		titlePos,			// スクリーンの表示位置(originの描画位置)
		nullptr,			// 矩形(RECT)
		Colors::White,		// 背景色
		0.0f,				// 回転角(ラジアン)
		m_texCenter1,		// テクスチャの基準になる表示位置(描画中心)(origin)
		1.0f,				// スケール(scale)
		SpriteEffects_None,	// エフェクト(effects)
		0.0f				// レイヤ深度(画像のソートで必要)(layerDepth)
	);

	// SPACEでスタート.png の描画位置を調整
	DirectX::SimpleMath::Vector2 pos2 = DirectX::SimpleMath::Vector2(pos.x, pos.y + 200.0f);

	// SPACEでスタート.png を中央に描画する
	m_spriteBatch->Draw(
		m_texture2.Get(),   // テクスチャ(SRV)
		pos2,               // スクリーンの表示位置(originの描画位置)
		nullptr,            // 矩形(RECT)
		Colors::White,      // 背景色
		0.0f,               // 回転角(ラジアン)
		m_texCenter2,       // テクスチャの基準になる表示位置(描画中心)(origin)
		1.0f,               // スケール(scale)
		SpriteEffects_None, // エフェクト(effects)
		0.0f                // レイヤ深度(画像のソートで必要)(layerDepth)
	);
}



//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void TitleScene::Finalize()
{
	// do nothing.
}

//---------------------------------------------------------
// 次のシーンIDを取得する
//---------------------------------------------------------
IScene::SceneID TitleScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		return IScene::SceneID::PLAY;
	}

	// シーン変更がない場合
	return IScene::SceneID::NONE;
}

