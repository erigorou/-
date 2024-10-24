#pragma once


namespace mylib
{
	class Texture
	{
	public:

		// ------------------------------------------------
		// テクスチャをロードする
		// ------------------------------------------------
		static void LoadTexture(
			ID3D11Device* device										,	// デバイス
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture	,	// テクスチャ
			const wchar_t* fileName											// ファイル名
		);

		// ------------------------------------------------
		// 画像の大きさを求め、テクスチャの中心座標を計算する
		// ------------------------------------------------
		static void CalculateTextureCenter(
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture	,	// テクスチャ
			DirectX::SimpleMath::Vector2& texSize						,	// テクスチャの大きさ
			DirectX::SimpleMath::Vector2& texCenter							// テクスチャの中心座標
		);
	};
};