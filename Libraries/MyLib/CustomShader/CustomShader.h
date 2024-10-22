#pragma once
#ifndef CUSTOMSHADER_DEFINED
#define CUSTOMSHADER_DEFINED

#include "pch.h"

class CustomShader
{
public:
	//	入力要素配列
	using InputElements = std::vector<D3D11_INPUT_ELEMENT_DESC>;


	ID3D11InputLayout* GetInputLayout() { return m_inputLayout.Get(); }

	// コンストラクタ
	CustomShader
	(
		ID3D11Device*	device			,
		const wchar_t*	vertexPath		,
		const wchar_t*	pixelPath		,
		const wchar_t*	geometryPath	,
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

private:
	// 頂点シェーダーの読み込み
	void LoadVertexShader(ID3D11Device* device, const wchar_t* filePath, const InputElements& inputElements);
	// ピクセルシェーダーの読み込み
	void LoadPixelShader(ID3D11Device* device, const wchar_t* filePath);
	// ジオメトリシェーダーの読み込み
	void LoadGeometryShader(ID3D11Device* device, const wchar_t* filePath);


	Microsoft::WRL::ComPtr<ID3D11VertexShader>		m_vertexShader;		// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		m_pixelShader;		// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>	m_geometryShader;	// ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		m_inputLayout;		// 入力レイアウト
};


#endif // CUSTOMSHADER_DEFINED