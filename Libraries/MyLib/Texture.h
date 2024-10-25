#pragma once

class CommonResources;

namespace mylib
{
	class Texture
	{
	public:

		// ------------------------------------------------
		// テクスチャのロード関数
		// ------------------------------------------------
		// テクスチャをロードする
		// device: デバイス
		// texture: テクスチャ
		// fileName: ファイル名
		// ------------------------------------------------
		static void LoadTexture(
			ID3D11Device* device										,	// デバイス
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture	,	// テクスチャ
			const wchar_t* fileName											// ファイル名
		);

		// ------------------------------------------------
		// 画像の大きさを求め、テクスチャの中心座標を計算する
		// ------------------------------------------------
		// テクスチャの大きさを求める
		// texture: テクスチャ
		// texSize: テクスチャの大きさ
		// texCenter: テクスチャの中心座標
		// ------------------------------------------------
		static void CalculateTextureCenter(
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture	,	// テクスチャ
			DirectX::SimpleMath::Vector2& texSize						,	// テクスチャの大きさ
			DirectX::SimpleMath::Vector2& texCenter							// テクスチャの中心座標
		);



		// ------------------------------------------------
		// テクスチャを描画する
		// ------------------------------------------------
		// spriteBatch: スプライトバッチ
		// position: 位置
		// texture: テクスチャ
		// texCenter: テクスチャの中心座標
		// scale: 拡大率
		// rotation: 回転角度
		// ------------------------------------------------
		static void DrawTexture(
			DirectX::SpriteBatch* spriteBatch,
			DirectX::SimpleMath::Vector2 position,
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture,
			DirectX::SimpleMath::Vector2 texCenter,
			DirectX::SimpleMath::Vector2 scale,
			float rotation
		);
	};
};