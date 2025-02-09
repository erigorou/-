// -------------------------------------
// 
// 名前;	Sea.h
// 内容:	海の描画を担当するクラス
// 作成:	池田桜輔
// 
// -------------------------------------

#ifndef SEA_H
#define SEA_H

#pragma once
// インクルード
#include "pch.h"

// 前方宣言
class CustomShader;

/// <summary>
/// 海の描画を担当するクラス
/// </summary>
class Sea
{
	// -----------------------------
	// 定数
	// -----------------------------
public:
	// 定数バッファ用データ構造体
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;  // ワールド行列
		DirectX::SimpleMath::Matrix matView;   // ビュー行列
		DirectX::SimpleMath::Matrix matProj;   // プロジェクション行列
		DirectX::SimpleMath::Vector4 diffuse;  // 拡散反射色
		DirectX::SimpleMath::Vector4 easing;   // イージング用のパラメータ
		DirectX::SimpleMath::Vector4 time;     // 時間関連のパラメータ
	};

	// 入力レイアウト（頂点フォーマット）定義
	std::vector<D3D11_INPUT_ELEMENT_DESC> InputElements =
	{
		// 頂点位置
		D3D11_INPUT_ELEMENT_DESC{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		// 頂点カラー
		D3D11_INPUT_ELEMENT_DESC{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		// テクスチャ座標
		D3D11_INPUT_ELEMENT_DESC{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// シェーダーのファイルパス
	static constexpr wchar_t VS_PATH[] = L"Resources/cso/VoronoiVS.cso";
	static constexpr wchar_t PS_PATH[] = L"Resources/cso/VoronoiPS.cso";
	static constexpr wchar_t GS_PATH[] = L"Resources/cso/VoronoiGS.cso";

	// -----------------------------
	// 公開メンバ関数
	// -----------------------------
public:
	// コンストラクタ
	Sea();
	// デストラクタ
	~Sea();

	// 描画処理
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	// -----------------------------
	// 非公開メンバ関数
	// -----------------------------
private:
	// リソースの生成処理
	void Create();

	// -----------------------------
	// 非公開メンバ変数
	// -----------------------------
private:
	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	// ステートオブジェクト
	std::unique_ptr<DirectX::CommonStates> m_states;
	// カスタムシェーダー
	std::unique_ptr<CustomShader> m_customShader;

	// 合計時間
	float m_totalTime;
};

#endif // SEA_H
