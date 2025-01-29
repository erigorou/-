//--------------------------------------------------------------------------------------
// File: UserInterface.h
//
// ユーザーインターフェースクラス
//
//-------------------------------------------------------------------------------------

#pragma once

#include "pch.h"
#include "DeviceResources.h"
#include "Game/UI/UIAnchor.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "Interface/IAction.h"


class UserInterface
{
// ------------------------------
// 定数
// ------------------------------
public:
	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector2 windowSize;
		float alpha;
		float padding;
	};
	// インプットレイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	// ウィンドウサイズ
	static constexpr DirectX::SimpleMath::Vector2 WINDOW_SIZE = DirectX::SimpleMath::Vector2(1280.0f, 720.0f);


// ------------------------------
// 公開関数
// ------------------------------
public:
	// コンストラクタ
	UserInterface();
	// デストラクタ
	~UserInterface();
	// UIの生成処理
	void Create(
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture,
		DirectX::SimpleMath::Vector2 position,
		DirectX::SimpleMath::Vector2 scale,
		ANCHOR anchor,
		IAction* action
	);
	// 更新処理
	void Update(const float time);
	// 描画処理
	void Render();


// ------------------------------
// 内部関数
// ------------------------------
private:
	// テクスチャのサイズを取得
	void GetTextureSize(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv);
	// シェーダーの生成
	void CreateShader();


// ------------------------------
// メンバ変数
// ------------------------------
private:
	// 経過時間
	float m_totalTime;
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	//コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	// ウィンドウのサイズ
	DirectX::SimpleMath::Vector2 m_windowSize;
	// テクスチャのサイズ
	DirectX::SimpleMath::Vector2 m_textureSize;
	// スケール
	DirectX::SimpleMath::Vector2 m_scale;
	// 座標
	DirectX::SimpleMath::Vector2 m_position;
	// アルファ値
	float m_alpha;
	// シェーダー
	std::unique_ptr<CustomShader> m_shader;
	// アンカー
	ANCHOR m_anchor;
	// アクション
	IAction* m_action;
	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
};