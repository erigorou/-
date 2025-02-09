// ------------------------------------------------------------------------------
// 名前:	WinResult.cpp
// 内容:	敗北時のリザルトシーンクラス
// 制作:	池田桜輔
// ------------------------------------------------------------------------------
#include "pch.h"
#include "WinResult.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/Texture.h"
#include "Game/GameResources.h"

//---------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// ---------------------------------------------------------
WinResult::WinResult()
	:
	m_spriteBatch{},
	m_texture{},
	m_texCenter{},
	m_isChangeScene{}
{
}

// ---------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------------------------
WinResult::~WinResult()
{
	// do nothing.
}

// ---------------------------------------------------------
/// <summary>
/// 初期化処理を行う
/// </summary>
// ---------------------------------------------------------
void WinResult::Initialize()
{
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// スプライトバッチを作成する
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

	// テクスチャの作成およびデータの取得
	m_texture = GameResources::GetInstance()->GetTexture("win");

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

// ---------------------------------------------------------	
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------------------
void WinResult::Update(float elapsedTime)
{
	// 宣言をしたが、実際は使用していない変数
	UNREFERENCED_PARAMETER(elapsedTime);
}

//---------------------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
// ---------------------------------------------------------
void WinResult::Render()
{
	auto states = CommonResources::GetInstance()->GetCommonStates();

	// スプライトバッチの開始：オプションでソートモード、ブレンドステートを指定する
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, states->NonPremultiplied());

	DirectX::SimpleMath::Vector2 texPos{ 0.0f, 200.0f };

	// テクスチャを描画する
	mylib::Texture::DrawTexture
	(
		m_spriteBatch.get(),
		texPos,
		m_texture,
		m_texCenter,
		DirectX::SimpleMath::Vector2{ 1.0f, 1.0f },
		0.0f
	);

	// スプライトバッチの終わり
	m_spriteBatch->End();
}

//---------------------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
// ---------------------------------------------------------
void WinResult::Finalize()
{
}

// ---------------------------------------------------------
/// <summary>
/// 次のシーンIDを取得する
/// </summary>
// ---------------------------------------------------------
IScene::SceneID WinResult::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		return IScene::SceneID::TITLE;
	}

	// シーン変更がない場合
	return IScene::SceneID::NONE;
}