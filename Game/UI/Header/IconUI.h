

// プリコンパイルヘッダー
#include "pch.h"
#include "DeviceResources.h"
#include "CommonStates.h"
#include "PrimitiveBatch.h"
#include "VertexTypes.h"
#include "Effects.h"
#include "SimpleMath.h"
#include "StepTimer.h"

//UIのアンカーポイントの列挙数
enum ANCHOR
{
	TOP_LEFT = 0,
	TOP_CENTER,
	TOP_RIGHT,

	MIDDLE_LEFT,
	MIDDLE_CENTER,
	MIDDLE_RIGHT,

	BOTTOM_LEFT,
	BOTTOM_CENTER,
	BOTTOM_RIGHT
};

// アイコンクラス
class IconUI
{
public:
	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4	windowSize;
	};

public:
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;


	void SetWindowSize(const int& width, const int& height);	// ウィンドウサイズの設定

	void SetScale(DirectX::SimpleMath::Vector2 scale);			// スケールの設定
	DirectX::SimpleMath::Vector2 GetScale() { return m_scale; }	// スケールの取得

	DirectX::SimpleMath::Vector2 GetBaseScale() { return m_baseScale; }	// ベーススケールの取得

	void SetPosition(DirectX::SimpleMath::Vector2 position);			// 座標の設定
	DirectX::SimpleMath::Vector2 GetPosition() { return m_position; }	// 座標の取得

	void SetAnchor(ANCHOR anchor);				// アンカーポイントの設定
	ANCHOR GetAnchor() { return m_anchor; }		// アンカーポイントの取得



	IconUI();		// コンストラクタ
	~IconUI();		// デストラクタ

	void LoadTexture(const wchar_t* path);	// テクスチャの読み込み

	void Create(DX::DeviceResources* pDR	// 初期化関数
		, const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, ANCHOR anchor);

	void Render();							// 描画関数

private:

	void CreateShader();

private:
	DX::DeviceResources* m_pDR;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

	DX::StepTimer                           m_timer;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	//コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	Microsoft::WRL::ComPtr<ID3D11Resource> m_res;
	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;

	int m_windowWidth, m_windowHeight;
	int m_textureWidth, m_textureHeight;
	int m_yoshiTextureWidth, m_yoshiTextureHeight;

	DirectX::SimpleMath::Vector2 m_scale;
	DirectX::SimpleMath::Vector2 m_baseScale;
	DirectX::SimpleMath::Vector2 m_position;

	ANCHOR m_anchor;

};