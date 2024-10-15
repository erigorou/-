#pragma once
#ifndef FADE_DEFINED
#define FADE_DEFINED

#include "pch.h"
#include "DeviceResources.h"
#include "Game/CommonResources.h"

class SceneManager;
class Fade
{
public:
	// フェードの状態
	enum class FadeType : UINT
	{
		FADE_IN,	// 暗　→　明
		FADE_OUT,	// 明　→　暗
		FADE_NONE,	// なし
	};


	//	データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
		DirectX::SimpleMath::Vector4	time;
	};

	//	関数
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	
	static const wchar_t* TEXTURE_PATH;
	static const wchar_t* VS_PATH;
	static const wchar_t* GS_PATH;
	static const wchar_t* PS_PATH;

	static constexpr float FADE_TIME = 1.0f;

	void FadeStart(FadeType type);		// フェード開始

	Fade(SceneManager* scene);			// コンストラクタ
	~Fade();							// デストラクタ
	void Initialize();					// 初期化処理
	void Update(float elapsedTime);		// 更新処理
	void Render();						// 描画処理


private:
	void LoadTexture(const wchar_t* path);		// テクスチャの読み込み
	void CreateShader();						// シェーダーの生成処理
	void CountTimer();							// タイマーのカウント処理

	void FadeIn();
	void FadeOut();

	SceneManager* m_scene;		// シーンマネージャ

	float		m_elapsedTime;	// 1Fの時間
	float		m_totalTime;	// 経過時間
	bool		m_isFade;		// フェード中かどうか
	FadeType	m_fadeType;		// フェードの状態

	DX::DeviceResources*						m_pDR;		// デバイスリソーシーズ
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_CBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_input_Layout;

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	std::unique_ptr<DirectX::CommonStates> m_states;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>	m_geometryShader;

};

#endif // !FADE_DEFINED