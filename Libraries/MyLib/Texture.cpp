#include "pch.h"
#include "Texture.h"



// ------------------------------------------------
// テクスチャをロードする
// ------------------------------------------------
void mylib::Texture::LoadTexture
	(
	ID3D11Device* device,
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture,
	const wchar_t* fileName
	)
{
	// 画像をロードする
	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(
			device,
			fileName,
			nullptr,
			texture.ReleaseAndGetAddressOf()
		)
	);
}


// ------------------------------------------------
// 画像の大きさを取得し、テクスチャの中心座標を計算する
// ------------------------------------------------
void mylib::Texture::CalculateTextureCenter
	(
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture	, 
	DirectX::SimpleMath::Vector2& texSize						, 
	DirectX::SimpleMath::Vector2& texCenter
	)
{
	Microsoft::WRL::ComPtr<ID3D11Resource> resource{};	// リソース
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D{};	// テクスチャ(2D版)
	D3D11_TEXTURE2D_DESC desc{};						// テクスチャの情報

	// テクスチャのリソースを取得（返還前）
	texture->GetResource(resource.GetAddressOf());
	// ID3D11ResourceをID3D11Texture2Dに変換
	resource.As(&tex2D);
	tex2D->GetDesc(&desc);
	// テクスチャの大きさを求める
	texSize.x = static_cast<float>(desc.Width);
	texSize.y = static_cast<float>(desc.Height);

	// テクスチャの中心座標を計算する
	texCenter = texSize / 2.0f;
}
