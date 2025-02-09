// ------------------------------------------------------------------------------
// 名前:	LoseResult.cpp
// 内容:	敗北時のリザルトシーンクラス
// 制作:	池田桜輔
// ------------------------------------------------------------------------------

#include "pch.h"
#include "LoseResult.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/Texture.h"
#include "Game/GameResources.h"

const wchar_t* LoseResult::TEXTURE_PATH = L"Resources/Textures/UI/Lose.png";

// ---------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// ---------------------------------------------
LoseResult::LoseResult()
	:
	m_spriteBatch{},
	m_texture{},
	m_texCenter{},
	m_isChangeScene{}
{
}

// ---------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------------
LoseResult::~LoseResult()
{
	Finalize();
}

// ---------------------------------------------
/// <summary>
/// 初期化する
/// </summary>
// ---------------------------------------------
void LoseResult::Initialize()
{
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// スプライトバッチを作成する
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

	// テクスチャを取得
	m_texture = GameResources::GetInstance()->GetTexture("lose");

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

// ---------------------------------------------
/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------
void LoseResult::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}

// ---------------------------------------------
/// <summary>
/// 描画する
/// </summary>
// ---------------------------------------------

void LoseResult::Render()
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

// ---------------------------------------------
/// <summary>
/// 後始末する
/// </summary>
// ---------------------------------------------
void LoseResult::Finalize()
{
}

// ---------------------------------------------
/// <summary>
/// 次のシーンIDを取得する
/// </summary>
/// <returns>次のシーンID</returns>
// ---------------------------------------------
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