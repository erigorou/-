// ----------------------------------------------------------------------------
//
// 名前:	CustomShader.cpp
// 内容:	カスタムシェーダークラス
//			シェーダーを実装する際の基本的な機能を提供する
//			読み込み・設定・解放など
// 制作:	池田桜輔
//
// ----------------------------------------------------------------------------
// インクルード
#include "pch.h"
#include "CustomShader.h"
#include "../BinaryFile.h"

/// <summary>
/// コンストラクタ(生成処理)
/// </summary>
/// <param name="device">デバイス情報</param>
/// <param name="vertexPath">頂点シェーダーのパス</param>
/// <param name="pixelPath">ピクセルシェーダーのパス</param>
/// <param name="geometryPath">ジオメトリシェーダーのパス</param>
/// <param name="inputElements">入力レイアウト</param>
CustomShader::CustomShader(
	ID3D11Device* device,
	const wchar_t* vertexPath,
	const wchar_t* pixelPath,
	const wchar_t* geometryPath,
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements
)
{
	//	頂点シェーダーの読み込み
	if (vertexPath != nullptr) { LoadVertexShader(device, vertexPath, inputElements); }
	//	ピクセルシェーダーの読み込み
	if (pixelPath != nullptr) { LoadPixelShader(device, pixelPath); }
	//	ジオメトリシェーダーの読み込み
	if (geometryPath != nullptr) { LoadGeometryShader(device, geometryPath); }
}

/// <summary>
/// テクスチャの読み込み
/// </summary>
/// <param name="device">デバイス情報</param>
/// <param name="path">ファイルパス</param>
/// <param name="textureArray">テクスチャの配列</param>
void CustomShader::LoadTexture(
	ID3D11Device* device,
	const wchar_t* path,
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& textureArray
)
{
	// テクスチャを保存する変数
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;

	// テクスチャの読み込み
	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(
			device,
			path,
			nullptr,
			texture.ReleaseAndGetAddressOf()
		)
	);

	// 配列に格納
	textureArray.push_back(texture);
}

/// <summary>
/// シェーダーを設定する
/// </summary>
/// <param name="context">デバイスコンテキスト</param>
void CustomShader::BeginSharder(ID3D11DeviceContext* context)
{
	//	作成されているシェーダーの設定
	if (m_vertexShader.Get() != nullptr) { context->VSSetShader(m_vertexShader.Get(), nullptr, 0); }
	if (m_pixelShader.Get() != nullptr) { context->PSSetShader(m_pixelShader.Get(), nullptr, 0); }
	if (m_geometryShader.Get() != nullptr) { context->GSSetShader(m_geometryShader.Get(), nullptr, 0); }

	//	作成されていれば入力レイアウトの登録
	if (m_inputLayout.Get() != nullptr)
	{
		context->IASetInputLayout(m_inputLayout.Get());
	}
}

/// <summary>
/// シェーダーの解放
/// </summary>
/// <param name="context">デバイスコンテキスト</param>
void CustomShader::EndSharder(ID3D11DeviceContext* context)
{
	//	シェーダの登録を解除
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
}

/// <summary>
/// 頂点シェーダーの読み込み
/// インプットレイアウトの作成
/// </summary>
/// <param name="device">デバイス情報</param>
/// <param name="filePath">頂点シェーダーのファイルパス</param>
/// <param name="inputElements">入力情報</param>
void CustomShader::LoadVertexShader(
	ID3D11Device* device, 
	const wchar_t* filePath,
	const InputElements& inputElements
)
{
	// ファイルの読み込み
	BinaryFile VSData = BinaryFile::LoadFile(filePath);

	// 頂点シェーダの作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), nullptr, m_vertexShader.GetAddressOf())))
		MessageBox(0, L"頂点シェーダの生成に失敗しました.", NULL, MB_OK);

	// 入力レイアウトを作成しなくていい
	if (inputElements.size() <= 0) { return; }

	// 入力レイアウトの作成(頂点シェーダーの情報が必要なためここで生成)
	device->CreateInputLayout(
		&inputElements.begin()[0],
		(UINT)inputElements.size(),
		VSData.GetData(),
		VSData.GetSize(),
		m_inputLayout.GetAddressOf()
	);
}

/// <summary>
/// ピクセルシェーダーの読み込み
/// </summary>
/// <param name="device">デバイス情報</param>
/// <param name="filePath">ピクセルシェーダーのファイルパス</param>
void CustomShader::LoadPixelShader(ID3D11Device* device, const wchar_t* filePath)
{
	//	バイナリファイルの読み込み
	BinaryFile PSData = BinaryFile::LoadFile(filePath);

	//	ピクセルシェーダの作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), nullptr, m_pixelShader.GetAddressOf())))
		MessageBox(0, L"ピクセルシェーダの生成に失敗しました.", NULL, MB_OK);
}

/// <summary>
/// ジオメトリシェーダーの読み込み
/// </summary>
/// <param name="device">デバイス情報</param>
/// <param name="filePath">ジオメトリシェーダーのファイルパス</param>
void CustomShader::LoadGeometryShader(ID3D11Device* device, const wchar_t* filePath)
{
	//	バイナリファイルの読み込み
	BinaryFile GSData = BinaryFile::LoadFile(filePath);

	//	ジオメトリシェーダの作成
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), nullptr, m_geometryShader.GetAddressOf())))
		MessageBox(0, L"ジオメトリシェーダの生成に失敗しました.", NULL, MB_OK);
}