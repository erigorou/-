// ---------------------------------------------------------------------------------------
// 名前:	Tutorial.h
// 内容:	チュートリアル中の言葉の部分を描画するクラス
// 作成:	池田桜輔
// ---------------------------------------------------------------------------------------
#pragma once
#include "pch.h"


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
	static constexpr float ANIMATION_TIME = 1.0f;

	// シェーダーのファイルパス
	static constexpr wchar_t VS_PATH[] = L"Resources/cso/TutorialVS.cso";
	static constexpr wchar_t PS_PATH[] = L"Resources/cso/TutorialPS.cso";
	static constexpr wchar_t GS_PATH[] = L"Resources/cso/TutorialGS.cso";

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
	// テクスチャの変更
	void ChangeTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture) { m_texture = texture; m_canUseTimer = true; }
	// タイマーフラグの取得
	bool GetTimerIsEnd() { return !m_timerPlay; }

	// --------------------
	// メンバ関数(公開)
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
	// タイマーの開始
	void StartTimer();
	// α値を減らす
	void StartAlphaMinus();

	// --------------------
	// メンバ関数(非公開)
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
	// メンバ変数
	// --------------------
private:
	// クエストマネージャー
	QuestManager* m_questManager;
	// 座標
	DirectX::SimpleMath::Vector3 m_position;
	// 回転角
	float m_angle;
	// 拡大率
	DirectX::SimpleMath::Vector2 m_scale;
	// 透明度
	float m_alpha;
	// 経過時間
	float m_currentTime;
	// 経過時間
	float m_elapsedTime;
	// タイマーフラグ
	bool m_timerPlay;
	// タイマーの使用フラグ
	bool m_canUseTimer;
	// α値を減らす
	bool m_alphaFlag;

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