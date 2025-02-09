//--------------------------------------------------------------------------------------
//
// 名前:	Fade.cpp
// 機能:	フェードイン、フェードアウトの処理を行う
// 作成:	池田桜輔
//
//-------------------------------------------------------------------------------------
#ifndef FADE_DEFINED
#define FADE_DEFINED

#pragma once
// インクルード
#include "pch.h"
#include "DeviceResources.h"
#include "Game/CommonResources.h"

// 前方宣言
class SceneManager;
class CustomShader;

/// <summary>
///	フェードクラス
/// </summary>
class Fade
{
	// -----------------------------
	// 固定値
	// -----------------------------
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

	// フェードの遅延時間
	static constexpr float CHANGE_SCENE_EDLAY = 1.0f;
	// フェードの時間
	static constexpr float FADE_TIME = 1.0f;
	// フェードの初期サイズ
	static constexpr float FADE_FIRST_SIZE = 1.25f;
	// フェードの最大サイズ
	static constexpr float FADE_MAX_SIZE = 6.0f;
	// フェードのしきい値
	static constexpr float FADE_THRESHOLD = 0.7f;

	// -----------------------------
	// メンバ関数(公開)
	// -----------------------------
public:
	// フェードインの開始
	void StartFadeIn();
	// フェードアウトの開始
	void StartFadeOut();
	// フェードの停止
	void FadeStop();

	// コンストラクタ
	Fade(SceneManager* scene);
	// デストラクタ
	~Fade();
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Render();

	// -----------------------------
	// メンバ関数(非公開)
	// -----------------------------
private:
	// シェーダーの生成処理
	void CreateShader();
	// タイマーのカウント処理
	void CountTimer();
	// フェードイン
	void FadeIn();
	// フェードアウト
	void FadeOut();
	// フェード終了
	void FadeEnd();
	// フェード量を計算する
	float CalcrateFadeValue(float easing);
	// 型抜き画像の描画処理
	void DrawStencilImage();

	// -----------------------------
	// メンバ変数
	// -----------------------------
private:
	// シーンマネージャ
	SceneManager* m_scene;
	// 1Fの時間
	float m_elapsedTime;
	// 経過時間
	float m_totalTime;
	// フェード時間
	float m_delayTime;
	// フェード中か
	bool m_isFade;
	// フェード終了か
	bool m_endFade;
	// フェードの状態
	FadeType m_fadeType;
	// イージング値
	float m_easingValue;
	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;
	// 頂点情報
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	// コモンステート
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