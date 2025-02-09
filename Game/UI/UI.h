// ------------------------------------------------------------------
//
// 名前: UI.h
// 機能: 画像を簡単に表示するクラス
// 製作: 池田桜輔
//
// ------------------------------------------------------------------
#pragma once
// インクルード
#include "pch.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"

/// <summary>
/// UIクラス
/// </summary>
class UI
{
	// -----------------------------
	// 固定値
	// -----------------------------
private:
	// シェーダーパス
	static constexpr wchar_t VS_PATH[] = L"Resources/cso/OperationVS.cso";
	static constexpr wchar_t PS_PATH[] = L"Resources/cso/OperationPS.cso";
	static constexpr wchar_t GS_PATH[] = L"Resources/cso/OperationGS.cso";

	// インプットレイアウト
	std::vector<D3D11_INPUT_ELEMENT_DESC> InputElements =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// フェード時間
	static constexpr float FADE_TIME = 1.2f;

	// 定数バッファ
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	diffuse;
		DirectX::SimpleMath::Vector4	easing;
		DirectX::SimpleMath::Vector4	time;
	};

	// -----------------------------
	// メンバ関数(公開)
	// -----------------------------
public:
	// コンストラクタ
	UI(std::string_view key);
	// デストラクタ
	~UI();
	// 初期化処理
	void Initialize();
	// 描画処理
	void Render();
	// 終了処理
	void Finalize();

	// -----------------------------
	// メンバ変数
	// -----------------------------
private:
	// シェーダー
	std::unique_ptr<CustomShader> m_customShader;
	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;
	// ステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
};