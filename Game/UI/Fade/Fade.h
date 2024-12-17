#pragma once
#ifndef FADE_DEFINED
#define FADE_DEFINED

#include "pch.h"
#include "DeviceResources.h"
#include "Game/CommonResources.h"

class SceneManager;
class CustomShader;

class Fade
{
public:
	// フェードの状態
	enum class FadeType : UINT
	{
		FADE_IN,	// 明　→　暗
		FADE_OUT,	// 暗　→　明
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

	static const wchar_t* STENCIL_TEX_PATH;
	static const wchar_t* MASK_VS_PATH;
	static const wchar_t* MASK_GS_PATH;
	static const wchar_t* MASK_PS_PATH;


	static constexpr float CHANGE_SCENE_TIME = 2.2f;
	static constexpr float FADE_TIME = 2.0f;


	void StartFadeIn();
	void StartFadeOut();
	void FadeStop();

	Fade(SceneManager* scene);			// コンストラクタ
	~Fade();							// デストラクタ
	void Initialize();					// 初期化処理
	void Update(float elapsedTime);		// 更新処理
	void Render();						// 描画処理


private:
	void CreateShader();						// シェーダーの生成処理
	void CountTimer();							// タイマーのカウント処理

	void FadeIn();
	void FadeOut();

	float CalcrateFadeValue(float t);
	float CalcurateFadeIn(float t);
	float CalcurateFadeOut(float t);

	void DrawStencilImage();					// 型抜き画像の描画処理

	SceneManager* m_scene;		// シーンマネージャ

	float		m_elapsedTime;	// 1Fの時間
	float		m_totalTime;	// 経過時間
	bool		m_isFade;		// フェード中かどうか
	FadeType	m_fadeType;		// フェードの状態

	float		m_easingValue;	// イージング値

	DX::DeviceResources*						m_pDR;		// デバイスリソーシーズ
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_CBuffer;	// コンスタントバッファ

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	std::unique_ptr<DirectX::CommonStates> m_states;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;

	// カスタムシェーダー
	std::unique_ptr<CustomShader> m_customShader;

	std::unique_ptr<CustomShader> m_maskShader;

	// 型抜き用画像
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_stencilImage;

	// 型抜き画像のみの描画結果を持つ画像
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_capture;
	// 型抜き画像のみの描画結果を持つ画像のビュー
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_captureRTV;
	// 型抜き画像のみの描画結果を持つ画像のシェーダーリソースビュー
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_captureSRV;

};

#endif // !FADE_DEFINED