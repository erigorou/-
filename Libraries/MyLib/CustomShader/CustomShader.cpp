
#include "pch.h"
#include "CustomShader.h"
#include "../BinaryFile.h"


// コンストラクタ
CustomShader::CustomShader
(
	ID3D11Device*									device,
	const wchar_t*									vertexPath,
	const wchar_t*									pixelPath,
	const wchar_t*									geometryPath,
	const std::vector<D3D11_INPUT_ELEMENT_DESC>&	inputElements
)
{
	//	頂点シェーダーの読み込み
	if (vertexPath != nullptr) { LoadVertexShader(device, vertexPath, inputElements); }
	//	ピクセルシェーダーの読み込み
	if (pixelPath != nullptr) { LoadPixelShader(device, pixelPath); }
	//	ジオメトリシェーダーの読み込み
	if (geometryPath != nullptr) { LoadGeometryShader(device, geometryPath); }
}


// テクスチャの読み込み
void CustomShader::LoadTexture(ID3D11Device* device, const wchar_t* path, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& textureArray)
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


// シェーダの開始
void CustomShader::BeginSharder(ID3D11DeviceContext* context)
{
	//	作成されているシェーダーの設定
	if (m_vertexShader		.Get()	!= nullptr) { context->VSSetShader(m_vertexShader.Get(), nullptr, 0); }
	if (m_pixelShader		.Get()	!= nullptr) { context->PSSetShader(m_pixelShader.Get(), nullptr, 0); }
	if (m_geometryShader	.Get()	!= nullptr) { context->GSSetShader(m_geometryShader.Get(), nullptr, 0); }

	//	作成されていれば入力レイアウトの登録
	if (m_inputLayout.Get() != nullptr) 
	{ context->IASetInputLayout(m_inputLayout.Get()); }
}


// シェーダの終了
void CustomShader::EndSharder(ID3D11DeviceContext* context)
{
	//	シェーダの登録を解除
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
}


// 頂点シェーダーの読み込み
void CustomShader::LoadVertexShader(ID3D11Device* device, const wchar_t* filePath, const InputElements& inputElements)
{
	//	バイナリファイルの読み込み
	BinaryFile VSData = BinaryFile::LoadFile(filePath);

	//	頂点シェーダの作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), nullptr, m_vertexShader.GetAddressOf())))
		MessageBox(0, L"頂点シェーダの生成に失敗しました.", NULL, MB_OK);

	//	入力レイアウトを作成しなくていい
	if (inputElements.size() <= 0) { return; }

	//	入力レイアウトの作成
	device->CreateInputLayout(
		&inputElements.begin()[0],
		(UINT)inputElements.size(),
		VSData.GetData(),
		VSData.GetSize(),
		m_inputLayout.GetAddressOf()
	);
}



// ピクセルシェーダーの読み込み
void CustomShader::LoadPixelShader(ID3D11Device* device, const wchar_t* filePath)
{
	//	バイナリファイルの読み込み
	BinaryFile PSData = BinaryFile::LoadFile(filePath);

	//	ピクセルシェーダの作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), nullptr, m_pixelShader.GetAddressOf())))
		MessageBox(0, L"ピクセルシェーダの生成に失敗しました.", NULL, MB_OK);
}



// ジオメトリシェーダーの読み込み
void CustomShader::LoadGeometryShader(ID3D11Device* device, const wchar_t* filePath)
{
	//	バイナリファイルの読み込み
	BinaryFile GSData = BinaryFile::LoadFile(filePath);

	//	ジオメトリシェーダの作成
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), nullptr, m_geometryShader.GetAddressOf())))
		MessageBox(0, L"ジオメトリシェーダの生成に失敗しました.", NULL, MB_OK);
}
