//--------------------------------------------------------------------------------------
// 
// 名前: UserInterface.cpp
// 機能: UIを表示するクラス
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
/// UIの表示を行うクラス
/// </summary>
class UserInterface : public IRenderable
{
	// ------------------------------
	// 定数
	// ------------------------------
public:
	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector2 windowSize;
		DirectX::SimpleMath::Vector2 padding;
		DirectX::SimpleMath::Vector4 color;
	};
	// インプットレイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	// シェーダーのファイルパ
	static constexpr wchar_t VS_PATH[] = L"Resources/cso/UIVS.cso";
	static constexpr wchar_t PS_PATH[] = L"Resources/cso/UIPS.cso";
	static constexpr wchar_t GS_PATH[] = L"Resources/cso/UIGS.cso";

	// -----------------------------
	// アクセサ
	// -----------------------------
public:
	// 描画レイを取得
	UINT GetLayer() const override { return m_layer; }
	// レイヤーの設定
	void SetLayer(const UINT layer){ m_layer = layer; }

	// -----------------------------
	// メンバ関数(公開)
	// -----------------------------
public:
	// コンストラクタ
	UserInterface();
	// デストラクタ
	~UserInterface();
	// UIの生成処理
	void Create(
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture,
		DirectX::SimpleMath::Vector2 position,
		DirectX::SimpleMath::Vector2 scale,
		ANCHOR anchor,
		IAction* action
	);
	// 更新処理
	void Update(const float time);
	// 描画処理
	void Render();
	// 描画コマンドを記録
	void RecordRenderCommands(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj,
		ID3D11DeviceContext* deferredContext) override;

	// -----------------------------
	// メンバ関数(非公開)
	// -----------------------------
private:
	// テクスチャのサイズを取得
	void GetTextureSize(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv);
	// シェーダーの生成
	void CreateShader();

	// ------------------------------
	// メンバ変数
	// ------------------------------
private:
	// 経過時間
	float m_totalTime;
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	// ウィンドウのサイズ
	DirectX::SimpleMath::Vector2 m_windowSize;
	// テクスチャのサイズ
	DirectX::SimpleMath::Vector2 m_textureSize;
	// スケール
	DirectX::SimpleMath::Vector2 m_scale;
	// 座標
	DirectX::SimpleMath::Vector2 m_position;
	// 色情報
	DirectX::SimpleMath::Vector4 m_color;
	// シェーダー
	std::unique_ptr<CustomShader> m_shader;
	// アンカー
	ANCHOR m_anchor;
	// アクション
	IAction* m_action;
	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	// レイヤー
	UINT m_layer = static_cast<UINT>(Layer::UI);
};