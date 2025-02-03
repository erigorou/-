#pragma once
#include "pch.h"
#include "DeviceResources.h"

class QuestManager;
class CustomShader;
class DeviceResources;

class Tutorial
{
	// --------------------
	// 固定値
	// --------------------
public:
	// テクスチャサイズの大きさ
	static constexpr int WIDTH = 1000;
	static constexpr int HEIGHT = 100;

	// 画面サイズの大きさ
	static constexpr float WINDOW_WIDTH = 1280;
	static constexpr float WINDOW_HEIGHT = 720;

	// テクスチャの初期座標
	static constexpr float INIT_POSITION_X = (WINDOW_WIDTH - WIDTH) / 2;
	static constexpr float INIT_POSITION_Y = 600.0f;

	// アニメーション時間
	static constexpr float ANIMATION_TIME = 2.0f;

	// シェーダーのファイルパス
	static const wchar_t* VS_PATH;
	static const wchar_t* PS_PATH;
	static const wchar_t* GS_PATH;

	// 入力レイアウト
	std::vector<D3D11_INPUT_ELEMENT_DESC> InputElements =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// 定数バッファ
	struct CBuffer
	{
		DirectX::SimpleMath::Vector4	windowSize;	// ウィンドウサイズ
		float							alpha;		// 透明度
		DirectX::SimpleMath::Vector3	padding;	// 空白
	};

	// --------------------
	// アクセサ
	// --------------------
public:
	void ChangeTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture) { m_texture = texture; m_canUseTimer = true; }	// テクスチャの変更
	bool GetTimerIsEnd() { return !m_timerPlay; }	// タイマーフラグの取得

	// --------------------
	// 公開関数
	// --------------------
public:
	// コンストラクタ
	Tutorial(QuestManager* manager);
	// デストラクタ
	~Tutorial();
	// 初期化処理
	void Initialize(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>);
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Draw();
	// 終了処理
	void Finalize();

	void StartTimer();	// タイマーの開始

	void StartAlphaMinus();

	// --------------------
	// 内部関数
	// --------------------
private:
	// シェーダーの生成
	void CreateShader();
	// コンスタントバッファの作成
	void ConstantBuffer();
	// 定数バッファの更新処理
	void UpdateConstantBuffer();
	// 描画設定
	void SetRenderState();

	// タイマーの更新処理
	void UpdateTimer(float elapsedTime);
	// α値の更新処理
	void UpdateAlpha(float elapsedTime);

	// --------------------
	// メンバー変数
	// --------------------
private:

	QuestManager* m_questManager;	// クエストマネージャー

	DirectX::SimpleMath::Vector3	m_position;	// 座標
	float							m_angle;	// 回転角
	DirectX::SimpleMath::Vector2	m_scale;	// 拡大率
	float							m_alpha;	// 透明度

	float	m_currentTime;	// 経過時間
	float	m_elapsedTime;	// 経過時間

	bool m_timerPlay;		// タイマーフラグ
	bool m_canUseTimer;		// タイマーの使用フラグ
	bool m_alphaFlag;		// α値を減らす

	// シェーダー
	std::unique_ptr<CustomShader> m_shader;
	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// ステート
	std::unique_ptr<DirectX::CommonStates>	m_states;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
};