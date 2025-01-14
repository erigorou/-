/*
	@file	QuestSelectScene.cpp
	@brief	タイトルシーンクラス
*/
#include "pch.h"
#include "QuestSelectScene.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/DebugString.h"
#include "../Factory/Factory.h"
#include "../Sound/Sound.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Libraries/MyLib/Math.h"
#include "Effects/Particle.h"
#include "Game/UI/UIAnchor.h"

#include "../Camera/Camera.h"
#include "../Stage/Floor/Floor.h"
#include "../Stage/Sea/Sea.h"
#include "../TitleObject/TitleEnemy.h"
#include "Libraries/MyLib/SkySphere.h"

#include <cassert>

using namespace DirectX;
using namespace DirectX::SimpleMath;

//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
QuestSelectScene::QuestSelectScene()
	:
	m_totalSeconds{},
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
QuestSelectScene::~QuestSelectScene()
{
	// do nothing.
}

//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void QuestSelectScene::Initialize()
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	// スプライトバッチを作成する
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);


	// テクスチャをロードする
	LoadTextures();

	// テクスチャの中心座標を計算する
	CalculateTextureCenters();

	// オブジェクトを生成する
	CreateObjects();

	// シーン変更フラグを初期化する
	m_isChangeScene = false;

	// 音楽の変更
	Sound::ChangeBGM(Sound::BGM_TYPE::TITLE);
}


//---------------------------------------------------------
// ロードする
//---------------------------------------------------------
void QuestSelectScene::LoadTextures()
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();

	// テクスチャを仮置きする
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> tex;

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

	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,
			L"Resources/Textures/Quest/TitleTutorial.png",
			nullptr,
			tex.ReleaseAndGetAddressOf()
		)
	);

	m_textureList.push_back(tex);

	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,
			L"Resources/Textures/Quest/TitleBossFight.png",
			nullptr,
			tex.ReleaseAndGetAddressOf()
		)
	);
	
	m_textureList.push_back(tex);
	
}


//---------------------------------------------------------
// テクスチャの中心を取得する
//---------------------------------------------------------
void QuestSelectScene::CalculateTextureCenters()
{
	CalculateTextureCenter(m_texture		, m_texCenter1);
	CalculateTextureCenter(m_texture2		, m_texCenter2);
	CalculateTextureCenter(m_textureList[0]	, m_texCenter3);
	CalculateTextureCenter(m_textureList[1]	, m_texCenter4);


	RECT rect{ m_commonResources->GetDeviceResources()->GetOutputSize() };

	// 射影行列を作成する
	m_projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(40.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 100000.0f
	);
}


void QuestSelectScene::CalculateTextureCenter(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture, DirectX::SimpleMath::Vector2& texCenter)
{
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D;
	D3D11_TEXTURE2D_DESC desc = {};

	// テクスチャの情報を取得
	texture->GetResource(resource.GetAddressOf());
	resource.As(&tex2D);
	tex2D->GetDesc(&desc);

	// テクスチャの中心を計算
	DirectX::SimpleMath::Vector2 texSize;
	texSize.x = static_cast<float>(desc.Width);
	texSize.y = static_cast<float>(desc.Height);
	texCenter = texSize / 2.0f;
}


//---------------------------------------------------------
// オブジェクトの生成
//---------------------------------------------------------
void QuestSelectScene::CreateObjects()
{
	m_camera	= Factory::CreateCamera	();
	m_floor		= Factory::CreateFloor	();
	m_sea = Factory::CreateSea();
	m_skySphere = Factory::CreateSkySphere();

	m_enemy = std::make_unique<TitleEnemy>();
	m_enemy->Initialize();
	m_skySphere->LoadSkySphereModel();

	// タイトルシーンのカメラステートを設定
	m_camera->ChangeState(m_camera->GetTitleState());

	m_particle = Factory::CreateParticle();
}


//---------------------------------------------------------
// 更新する
//---------------------------------------------------------
void QuestSelectScene::Update(float elapsedTime)
{
	m_totalSeconds += elapsedTime;

	// キーボードステートトラッカーを取得する
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();

	DirectX::SimpleMath::Vector3 zeroV = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Matrix zeroM = DirectX::SimpleMath::Matrix::Identity;

	m_camera->Shake(elapsedTime);
	m_camera->Update(zeroV, zeroV, zeroM, elapsedTime);

	m_enemy->Update(elapsedTime);

	// スペースキーが押されたら
	if (kbTracker->pressed.Space)
	{
		if (!m_isChangeScene)
		{


			//m_isChangeScene = true;
			//Sound::PlaySE(Sound::SE_TYPE::SYSTEM_OK);
		}
	}

	m_particle->Update(elapsedTime, DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero);
}

//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void QuestSelectScene::Render()
{
	auto states		= m_commonResources	->	GetCommonStates		();
	auto view		= m_camera			->	GetViewMatrix		();

	// オブジェクトの描画
	m_floor->		Render(view, m_projection);
	m_sea->			Render(view, m_projection);
	m_enemy->		Render(view, m_projection);
	m_skySphere->	DrawSkySphere(view, m_projection);

	// スプライトバッチの開始：オプションでソートモード、ブレンドステートを指定する
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states->NonPremultiplied());

	// テクスチャの描画
	DrawTexture();

	// パーティクルの描画
	m_particle->CreateBillboard(m_camera->GetEyePosition(),DirectX::SimpleMath::Vector3::Zero,m_camera->GetUpVector());
	m_particle->Render(view, m_projection);

	DrawStageSelect();

	// スプライトバッチの終わり
	m_spriteBatch->End();

#ifdef _DEBUG
#endif // _DEBUG
}


//---------------------------------------------------------
// テクスチャの描画
//---------------------------------------------------------
void QuestSelectScene::DrawTexture()
{
	RECT rect{ m_commonResources->GetDeviceResources()->GetOutputSize() };

	// 画像の中心を計算する
	Vector2 pos{ rect.right / 2.0f, rect.bottom / 2.0f };
	Vector2 titlePos = pos;
	titlePos.y += TITLE_LOGO_CENTER_Y;

	// タイトルロゴの上下移動の総量
	float moveValue = titlePos.y * 2;

	// 秒数を正規化
	float t = Math::Clamp(m_totalSeconds - DELAY, 0.0f, ANIM_END) / ANIM_END;

	// タイトルロゴの描画位置を決める（移動も考慮）
	Vector2 logoPos
	{
		titlePos.x ,
		TITLE_LOGO_CENTER_Y + moveValue * Easing::easeOutElastic(t)
	};


	// LOGO.png を中央に描画する
	m_spriteBatch->Draw(
		m_texture.Get(),	// テクスチャ(SRV)
		logoPos,			// スクリーンの表示位置(originの描画位置)
		nullptr,			// 矩形(RECT)
		Colors::White,		// 背景色
		0.0f,				// 回転角(ラジアン)
		m_texCenter1,		// テクスチャの基準になる表示位置(描画中心)(origin)
		1.0f,				// スケール(scale)
		SpriteEffects_None,	// エフェクト(effects)
		0.0f				// レイヤ深度(画像のソートで必要)(layerDepth)
	);

	// SPACEでスタート.png の描画位置を調整
	DirectX::SimpleMath::Vector2 pos2 = DirectX::SimpleMath::Vector2(pos.x , pos.y + 325.0f);

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


void QuestSelectScene::DrawStageSelect()
{
	RECT rect{ m_commonResources->GetDeviceResources()->GetOutputSize() };

	// 画像の中心を計算する
	Vector2 pos{ rect.right / 2.0f, rect.bottom / 2.0f };
	Vector2 titlePos = pos;

	// 移動量;
	float moveValue = m_texCenter3.x * 2;

	// 秒数を正規化
	float t = Math::Clamp(m_totalSeconds - STAGE_SELECT_DELAY, 0.0f, STAGE_SELECT_END) / STAGE_SELECT_END;

	// タイトルロゴの描画位置を決める（移動も考慮）
	Vector2 tutorialPos
	{
		WINDOW_WIDTH + m_texCenter3.x - moveValue * Easing::easeOutBounce(t),
		450.0f
	};


	// LOGO.png を中央に描画する
	m_spriteBatch->Draw(
		m_textureList[0].Get(),	// テクスチャ(SRV)
		tutorialPos,			// スクリーンの表示位置(originの描画位置)
		nullptr,				// 矩形(RECT)
		Colors::White,			// 背景色
		0.0f,					// 回転角(ラジアン)
		m_texCenter3,			// テクスチャの基準になる表示位置(描画中心)(origin)
		1.0f,					// スケール(scale)
		SpriteEffects_None,		// エフェクト(effects)
		0.0f					// レイヤ深度(画像のソートで必要)(layerDepth)
	);


	// タイトルロゴの描画位置を決める（移動も考慮）
	Vector2 BossFightPos
	{
		WINDOW_WIDTH + m_texCenter3.x - moveValue * Easing::easeOutBounce(t),
		575.0f
	};



	// SPACEでスタート.png を中央に描画する
	m_spriteBatch->Draw(
		m_textureList[1].Get(), // テクスチャ(SRV)
		BossFightPos,			// スクリーンの表示位置(originの描画位置)
		nullptr,				// 矩形(RECT)
		Colors::White,			// 背景色
		0.0f,					// 回転角(ラジアン)
		m_texCenter4,			// テクスチャの基準になる表示位置(描画中心)(origin)
		1.0f,					// スケール(scale)
		SpriteEffects_None,		// エフェクト(effects)
		0.0f					// レイヤ深度(画像のソートで必要)(layerDepth)
	);
}



//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void QuestSelectScene::Finalize()
{
	// do nothing.
}

//---------------------------------------------------------
// 次のシーンIDを取得する
//---------------------------------------------------------
IScene::SceneID QuestSelectScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		return IScene::SceneID::PLAY;
	}

	// シーン変更がない場合
	return IScene::SceneID::NONE;
}


//---------------------------------------------------------
// カメラを揺らす
//---------------------------------------------------------
void QuestSelectScene::SetShakeCamera()
{
 	m_camera->SetShake(1.0f);
}


void QuestSelectScene::CleateSpamDust(DirectX::SimpleMath::Vector3 pos)
{
	m_particle->CreateSlamDust(pos);
}
