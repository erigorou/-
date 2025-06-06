//--------------------------------------------------------------------------------------
// 
// 名前: ScreenShotUserInterface.cpp
// 機能: スクリーンショットのUIを表示するクラス
// 製作: 池田桜輔
//
//-------------------------------------------------------------------------------------
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "CommonStates.h"
#include "ScreenShotUserInterface.h"
#include "Game/Data/GameData.h"

#include "Libraries/MyLib/ThreadedRenderer/ThreadedRenderer.h"

/// <summary>
/// コンストラクタ
/// </summary>
ScreenShotUserInterface::ScreenShotUserInterface()
{
	// マルチスレッドに登録
	ThreadedRenderer::GetInstance()->RegisterRenderable(this);
}

/// <summary>
/// UI作成
/// </summary>
/// <param name="position">座標</param>
/// <param name="rotation">回転</param>
/// <param name="center">中心</param>
/// <param name="size">サイズ</param>
void ScreenShotUserInterface::Create(
	const DirectX::SimpleMath::Vector2& position,
	const float rotation,
	const DirectX::SimpleMath::Vector2& center,
	const DirectX::SimpleMath::Vector2& size
)
{
	m_position = position;
	m_rotation = rotation;
	m_center = center;
	m_size = size;

	// レイヤーの設定
	m_layer = 350;
}

/// <summary>
/// デストラクタ
/// </summary>
ScreenShotUserInterface::~ScreenShotUserInterface()
{
	// マルチスレッドから登録解除
	ThreadedRenderer::GetInstance()->UnregisterRenderable(this);
}

/// <summary>
/// 描画コマンドの記録
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
/// <param name="deferredContext">遅延コンテキスト</param>
void ScreenShotUserInterface::RecordRenderCommands(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj,
	ID3D11DeviceContext* deferredContext)
{
	UNREFERENCED_PARAMETER(view);
	UNREFERENCED_PARAMETER(proj);

	auto spriteBatch = DirectX::SpriteBatch(deferredContext);
	auto texture = GameData::GetInstance()->GetScreenShot();
	auto states = CommonResources::GetInstance()->GetCommonStates();

	if (texture)
	{
		// スプライトバッチの開始
		spriteBatch.Begin( DirectX::SpriteSortMode_Deferred, states->NonPremultiplied() );

		spriteBatch.Draw(
			texture.Get(),
			m_position,
			nullptr,
			DirectX::Colors::White,
			m_rotation,
			m_center,
			m_size,
			DirectX::SpriteEffects_None,
			0.0f
		);

		// スプライトバッチの終了
		spriteBatch.End();
	}
}
