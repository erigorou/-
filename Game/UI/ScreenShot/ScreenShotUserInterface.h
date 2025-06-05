//--------------------------------------------------------------------------------------
// 
// 名前: ScreenShotUserInterface.h
// 機能: スクリーンショットのUIを表示するクラス
// 製作: 池田桜輔
//
//-------------------------------------------------------------------------------------
#pragma once
// インクルード
#include "pch.h"
#include "DeviceResources.h"
#include "Game/UI/UIAnchor.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "Interface/IAction.h"
#include "Interface/IRenderable.h"


/// <summary>
/// スクリーンショットのUIを表示するクラス
/// </summary>
class ScreenShotUserInterface final : public IRenderable
{
	// ------------------------------
	// 定数
	// ------------------------------
public:


	// ------------------------------
	// アクセサ
	// ------------------------------
public:

	// 描画レイを取得
	UINT GetLayer() const override { return m_layer; }
	// レイヤーの設定
	void SetLayer(const UINT layer) { m_layer = layer; }

	// ------------------------------
	// メンバ関数(公開)
	// ------------------------------
public:
	// コンストラクタ
	ScreenShotUserInterface();
	// デストラクタ
	~ScreenShotUserInterface() override;
	// UIの生成処理
	void Create(
		const DirectX::SimpleMath::Vector2& position,
		const float rotation,
		const DirectX::SimpleMath::Vector2& center,
		const DirectX::SimpleMath::Vector2& size);

	// 描画コマンドを記録する
	void RecordRenderCommands(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj,
		ID3D11DeviceContext* deferredContext) override;


	// ---------------------------------
	// メンバ関数(非公開)
	// ---------------------------------
private:
	// テクスチャのサイズを取得
	void GetTextureSize(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv);

	// ---------------------------------
	// メンバ変数
	// ---------------------------------
private:
	// 座標
	DirectX::SimpleMath::Vector2 m_position;
	// 回転
	float m_rotation = 0.0f;
	// 中心
	DirectX::SimpleMath::Vector2 m_center;
	// サイズ
	DirectX::SimpleMath::Vector2 m_size;

	// レイヤー
	UINT m_layer = static_cast<UINT>(Layer::UI);

};

