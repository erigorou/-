// ----------------------------------------------------------------------------
//
// 名前:	CustomShader.cpp
// 内容:	カスタムシェーダークラス
//			シェーダーを実装する際の基本的な機能を提供する
//			読み込み・設定・解放など
// 制作:	池田桜輔
//
// ----------------------------------------------------------------------------
#pragma once
#ifndef CUSTOMSHADER_DEFINED
#define CUSTOMSHADER_DEFINED
// インクルード
#include "pch.h"

/// <summary>
/// カスタムシェーダークラス
/// </summary>
class CustomShader
{
	// -----------------------------
	// 固定値
	// -----------------------------
public:
	//	入力要素配列
	using InputElements = std::vector<D3D11_INPUT_ELEMENT_DESC>;

	// -----------------------------
	// メンバ関数(公開)
	// -----------------------------
public:
	// コンストラクタ
	CustomShader
	(
		ID3D11Device* device,
		const wchar_t* vertexPath,
		const wchar_t* pixelPath,
		const wchar_t* geometryPath,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements
	);
	//　画像読み取り処理
	static void LoadTexture
	(
		ID3D11Device* device,
		const wchar_t* path,
		std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& textureArray
	);

	// デストラクタ
	~CustomShader() = default;
	// シェーダを設定
	void BeginSharder(ID3D11DeviceContext* context);
	// シェーダーを取り消し
	void EndSharder(ID3D11DeviceContext* context);

	// -----------------------------
	// メンバ関数(非公開)
	// -----------------------------
private:
	// 頂点シェーダーの読み込み
	void LoadVertexShader(ID3D11Device* device, const wchar_t* filePath, const InputElements& inputElements);
	// ピクセルシェーダーの読み込み
	void LoadPixelShader(ID3D11Device* device, const wchar_t* filePath);
	// ジオメトリシェーダーの読み込み
	void LoadGeometryShader(ID3D11Device* device, const wchar_t* filePath);

	// -----------------------------
	// メンバ変数
	// -----------------------------
private:
	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};

#endif // CUSTOMSHADER_DEFINED