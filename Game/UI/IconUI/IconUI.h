//--------------------------------------------------------------------------------------
//
// 名前:	IconUI.h
// 内容:	アイコンUIのクラス
//			画像の描画などを簡単に行える
// 作成:	池田桜輔
//
//-------------------------------------------------------------------------------------
// インクルード
#include "pch.h"
#include "DeviceResources.h"
#include "Game/UI/UIAnchor.h"

// 前方宣言
class CustomShader;

// アイコンクラス
class IconUI
{
public:
	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4	windowSize;
	};

	// シェーダーパス
	static constexpr wchar_t VS_PATH[] = L"Resources/cso/UIVS.cso";
	static constexpr wchar_t PS_PATH[] = L"Resources/cso/UIPS.cso";
	static constexpr wchar_t GS_PATH[] = L"Resources/cso/UIGS.cso";

	// インプットレイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	// -----------------------------
	// メンバ関数(公開)
	// -----------------------------
public:



	// コンストラクタ
	IconUI();
	// デストラクタ
	~IconUI();
	// 生成処理
	void Create(
		DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, ANCHOR anchor
	);
	// 描画関数
	void Render();

	// -----------------------------
	// メンバ関数(非公開)
	// -----------------------------
private:
	// シェーダーの作成
	void CreateShader();

	// -----------------------------
	// メンバ変数
	// -----------------------------
private:
	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	//コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// カスタムシェーダー
	std::unique_ptr<CustomShader> m_customShader;
	// ウィンドウサイズ
	int m_windowWidth, m_windowHeight;
	// テクスチャサイズ
	int m_textureWidth, m_textureHeight;
	// テクスチャのスケール
	DirectX::SimpleMath::Vector2 m_scale;
	// テクスチャのベーススケール
	DirectX::SimpleMath::Vector2 m_baseScale;
	// テクスチャの座標
	DirectX::SimpleMath::Vector2 m_position;
	// アンカーポイント
	ANCHOR m_anchor;
};