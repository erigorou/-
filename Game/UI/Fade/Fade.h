#pragma once

#include "pch.h"
#include "DeviceResources.h"



class Fade
{
public:

	//	データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
	};

	//	関数
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	static const wchar_t* TEXTURE_PATH;

	static constexpr float FADE_TIME = 3.0f;

	Fade();	// コンストラクタ
	~Fade();	// デストラクタ

	void Initialize();	// 初期化処理
	void Update();		// 更新処理
	void Render();		// 描画処理

private:
	void LoadTexture(const wchar_t* path);		// テクスチャの読み込み
	void CreateShader();						// シェーダーの生成処理
	void CountTimer();

	float m_totalTime;

	DX::DeviceResources*						m_pDR;		// デバイスリソーシーズ
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_CBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_input_Layout;

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m__batch;
	std::unique_ptr<DirectX::CommonStates> m_states;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>	m_geometryShader;

};