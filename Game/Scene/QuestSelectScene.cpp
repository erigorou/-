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
#include "../Data/GameData.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Libraries/MyLib/Math.h"
#include "Effects/Particle.h"
#include "Game/UI/UIAnchor.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"

#include "../Camera/Camera.h"
#include "../Stage/Floor/Floor.h"
#include "../Stage/Sea/Sea.h"
#include "../TitleObject/TitleEnemy.h"
#include "Libraries/MyLib/SkySphere.h"

#include <cassert>

//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
QuestSelectScene::QuestSelectScene()
	:
	m_totalSeconds{},
	m_spriteBatch{},
	m_texture{},
	m_texCenter{},
	m_isChangeScene{},
	m_selectIndex{}
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
		DirectX::CreateWICTextureFromFile(
			device,
			L"Resources/Textures/LOGO.png",
			nullptr,
			m_texture.ReleaseAndGetAddressOf()
		)
	);

	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(
			device,
			L"Resources/Textures/SPACEでスタート.png",
			nullptr,
			m_texture2.ReleaseAndGetAddressOf()
		)
	);

	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(
			device,
			L"Resources/Textures/Quest/TitleTutorial.png",
			nullptr,
			tex.ReleaseAndGetAddressOf()
		)
	);

	m_textureList.push_back(tex);


	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(
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
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		DirectX::XMConvertToRadians(40.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 100000.0f
	);
}

// ---------------------------------------------------------
// テクスチャの中心を取得する
// ---------------------------------------------------------
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
	m_sea		= Factory::CreateSea();
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

	// オブジェクトの更新
	m_camera->Shake(elapsedTime);
	m_camera->Update(zeroV, zeroV, zeroM, elapsedTime);
	m_enemy->Update(elapsedTime);

	// パーティクルの更新
	m_particle->Update(elapsedTime, DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero);

	// ステージ選択
	SelectStage(kbTracker.get());
}



void QuestSelectScene::SelectStage(DirectX::Keyboard::KeyboardStateTracker* keyboard)
{
	if (m_isChangeScene) return;

	// スペースキーが押されたら
	if (keyboard->pressed.Space)
	{
		if (!m_isChangeScene)
		{
			// ゲームデータにステージを設定
			auto gameData = GameData::GetInstance();
			gameData->SetSelectStage(m_selectIndex);

			// シーン変更フラグを立てるのと音楽を鳴らす
			m_isChangeScene = true;
			Sound::PlaySE(Sound::SE_TYPE::SYSTEM_OK);
		}
	}

	// ステージ選択
	if (keyboard->pressed.Up	)	m_selectIndex = Math::Clamp(m_selectIndex - 1, MIN_STAGE_INDEX, MAX_STAGE_INDEX);
	if (keyboard->pressed.Down	)	m_selectIndex = Math::Clamp(m_selectIndex + 1, MIN_STAGE_INDEX, MAX_STAGE_INDEX);
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
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, states->NonPremultiplied());

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

	m_spriteBatch->Draw(
		m_texture.Get(),			// テクスチャ(SRV)
		logoPos,					// スクリーンの表示位置(originの描画位置)
		nullptr,					// 矩形(RECT)
		DirectX::Colors::White,		// 背景色
		0.0f,						// 回転角(ラジアン)
		m_texCenter1,				// テクスチャの基準になる表示位置(描画中心)(origin)
		1.0f,						// スケール(scale)
		DirectX::SpriteEffects_None,// エフェクト(effects)
		0.0f						// レイヤ深度(画像のソートで必要)(layerDepth)
	);

	// SPACEでスタート.png の描画位置を調整
	DirectX::SimpleMath::Vector2 pos2 = DirectX::SimpleMath::Vector2(pos.x , pos.y + 325.0f);

	// SPACEでスタート.png を中央に描画する
	m_spriteBatch->Draw(
		m_texture2.Get(),			// テクスチャ(SRV)
		pos2,						// スクリーンの表示位置(originの描画位置)
		nullptr,					// 矩形(RECT)
		DirectX::Colors::White,		// 背景色
		0.0f,						// 回転角(ラジアン)
		m_texCenter2,				// テクスチャの基準になる表示位置(描画中心)(origin)
		1.0f,						// スケール(scale)
		DirectX::SpriteEffects_None, // エフェクト(effects)
		0.0f						// レイヤ深度(画像のソートで必要)(layerDepth)
	);
}


void QuestSelectScene::DrawStageSelect()
{
	// ステージに応じた色情報の設定
	float colorRGB1 = 1.0f - 0.5f * m_selectIndex;
	float colorRGB2 = 0.5f + 0.5f * m_selectIndex;

	DirectX::SimpleMath::Vector4 color1 = DirectX::SimpleMath::Vector4(colorRGB1, colorRGB1, colorRGB1, 1.0f);
	DirectX::SimpleMath::Vector4 color2 = DirectX::SimpleMath::Vector4(colorRGB2, colorRGB2, colorRGB2, 1.0f);

	RECT rect{ m_commonResources->GetDeviceResources()->GetOutputSize() };

	// 画像の中心を計算する
	Vector2 pos{ rect.right / 2.0f, rect.bottom / 2.0f };
	Vector2 titlePos = pos;

	// 移動量;
	float moveValue = m_texCenter3.x * 3;

	// 秒数を正規化
	float t = Math::Clamp(m_totalSeconds - STAGE_SELECT_DELAY, 0.0f, STAGE_SELECT_END) / STAGE_SELECT_END;


	// タイトルロゴの描画位置を決める（移動も考慮）
	Vector2 tutorialPos
	{
		WINDOW_WIDTH + m_texCenter3.x * 2.0f - moveValue * Easing::easeOutBack(t),
		450.0f
	};


	// チュートリアルを描画
	m_spriteBatch->Draw(
		m_textureList[0].Get(),		// テクスチャ(SRV)
		tutorialPos,				// スクリーンの表示位置(originの描画位置)
		nullptr,					// 矩形(RECT)
		color1,						// 背景色
		0.0f,						// 回転角(ラジアン)
		m_texCenter3,				// テクスチャの基準になる表示位置(描画中心)(origin)
		1.0f,						// スケール(scale)
		DirectX::SpriteEffects_None,// エフェクト(effects)
		0.0f						// レイヤ深度(画像のソートで必要)(layerDepth)
	);


	float t2 = Math::Clamp(m_totalSeconds - STAGE_SELECT_DELAY - 0.2f, 0.0f, STAGE_SELECT_END) / STAGE_SELECT_END;

	// 本戦を描画
	Vector2 BossFightPos
	{
		WINDOW_WIDTH + m_texCenter4.x * 2.0f - moveValue * Easing::easeOutBack(t2),
		575.0f
	};



	// SPACEでスタート.png を中央に描画する
	m_spriteBatch->Draw(
		m_textureList[1].Get(),		// テクスチャ(SRV)
		BossFightPos,				// スクリーンの表示位置(originの描画位置)
		nullptr,					// 矩形(RECT)
		color2,						// 背景色
		0.0f,						// 回転角(ラジアン)
		m_texCenter4,				// テクスチャの基準になる表示位置(描画中心)(origin)
		1.0f,						// スケール(scale)
		DirectX::SpriteEffects_None,// エフェクト(effects)
		0.0f						// レイヤ深度(画像のソートで必要)(layerDepth)
	);
}



//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void QuestSelectScene::Finalize()
{
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


//---------------------------------------------------------
// シェーダーの生成
//---------------------------------------------------------
void QuestSelectScene::CreateShader(const wchar_t* VS, const wchar_t* PS, const wchar_t* GS)
{
	// デバイスの取得
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();

	// シェーダーの生成
	m_shaderList.push_back(std::make_unique<CustomShader>
		(
			device,
			VS,
			PS,
			GS,
			InputElements
		)
	);
}


//---------------------------------------------------------
// 定数バッファの作成
//---------------------------------------------------------
void QuestSelectScene::CreateConstantBuffer()
{
	// デバイスの取得
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();

	//	シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC cbDesc = {};
	cbDesc.ByteWidth = sizeof(CBuffer);
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;

	// コンスタントバッファの作成
	DX::ThrowIfFailed(
		device->CreateBuffer(&cbDesc, nullptr, m_CBuffer.GetAddressOf())
	);
}


// --------------------
// レンダーステートの設定
// --------------------
void QuestSelectScene::SetRenderState()
{
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = CommonResources::GetInstance()->GetCommonStates();

	//	画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	ID3D11BlendState* blendstate = states->NonPremultiplied(); 	//	半透明描画指定
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);		// 透明処理
	context->OMSetDepthStencilState(states->DepthNone(), 0);		// 深度バッファに書き込み参照しない
	context->RSSetState(states->CullClockwise());					// カリングは左回り
}



void QuestSelectScene::CleateSpamDust(DirectX::SimpleMath::Vector3 pos)
{
	m_particle->CreateSlamDust(pos);
}
