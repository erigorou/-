// ------------------------------------------------------------------
// 
// 名前:	PlayerWarning.h
// 機能:	HPが一定値以下になった時に警告を表示するクラス
// 製作:	池田桜輔
// 
// ------------------------------------------------------------------
#ifndef PLAYER_WARNING_DEFINED
#define PLAYER_WARNING_DEFINED
#pragma once
// インクルード
#include "pch.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"

// 前方宣言
class HPSystem;

/// <summary>
/// プレイヤーHPが一定値以下になった時に警告を表示するクラス
/// </summary>
class PlayerWarning
{
	// -----------------------------
	// 固定値
	// -----------------------------
private:
	// ファイルパス
	static constexpr wchar_t VS_PATH[] = L"Resources/cso/WarningVS.cso";
	static constexpr wchar_t PS_PATH[] = L"Resources/cso/WarningPS.cso";
	static constexpr wchar_t GS_PATH[] = L"Resources/cso/WarningGS.cso";

	// 警告を表示するHPの値
	static constexpr int LOW_HP = 2;
	// フェード時間
	static constexpr float FADE_TIME = 1.2f;

	// インプットレイアウト
	std::vector<D3D11_INPUT_ELEMENT_DESC> InputElements =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

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
	PlayerWarning(HPSystem* hp);
	// デストラクタ
	~PlayerWarning();
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Render();
	// 終了処理
	void Finalize();

	// -----------------------------
	// メンバ変数
	// -----------------------------
private:
	// HPシステム
	HPSystem* m_hp;
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
	// フレーム時間
	float m_elapsedTime;
	// 経過時間
	float m_totalTime;
};

#endif // WARNING_DEFINED
