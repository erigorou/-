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
		END_DELAY,	// フェード終了
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

	// インプットレイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	// フェードに使用するパス
	static constexpr wchar_t VS_PATH[] = L"Resources/cso/FadeVS.cso";
	static constexpr wchar_t GS_PATH[] = L"Resources/cso/FadeGS.cso";
	static constexpr wchar_t PS_PATH[] = L"Resources/cso/FadePS.cso";

	// マスクに使用するパス
	static constexpr wchar_t MASK_VS_PATH[] = L"Resources/cso/MaskVS.cso";
	static constexpr wchar_t MASK_GS_PATH[] = L"Resources/cso/MaskGS.cso";
	static constexpr wchar_t MASK_PS_PATH[] = L"Resources/cso/MaskPS.cso";


	// フェードの設定
	static constexpr float CHANGE_SCENE_EDLAY = 1.0f;
	static constexpr float FADE_TIME = 1.0f;

	static constexpr float FADE_FIRST_SIZE = 1.25f;
	static constexpr float FADE_MAX_SIZE = 6.0f;
	static constexpr float FADE_THRESHOLD = 0.7f;


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

	void FadeEnd();

	float CalcrateFadeValue(float t);

	void DrawStencilImage();					// 型抜き画像の描画処理

	SceneManager* m_scene;		// シーンマネージャ

	float		m_elapsedTime;	// 1Fの時間
	float		m_totalTime;	// 経過時間
	float		m_delayTime;		// フェード時間
	bool		m_isFade;		// フェード中かどうか
	bool		m_endFade;		// フェード終了かどうか
	FadeType	m_fadeType;		// フェードの状態

	float		m_easingValue;	// イージング値


	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_CBuffer;	// コンスタントバッファ

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	std::unique_ptr<DirectX::CommonStates> m_states;

	// カスタムシェーダー
	std::unique_ptr<CustomShader> m_customShader;
	// マスクシェーダー
	std::unique_ptr<CustomShader> m_maskShader;

	// ロード画面として使用するテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// マスク用のテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stencilImage;

	// 型抜き画像のみの描画結果を持つ画像
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_capture;
	// 型抜き画像のみの描画結果を持つ画像のビュー
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_captureRTV;
	// 型抜き画像のみの描画結果を持つ画像のシェーダーリソースビュー
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_captureSRV;

};

#endif // !FADE_DEFINED