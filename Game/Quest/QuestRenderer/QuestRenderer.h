#pragma once
#include "pch.h"
#include "QuestRenderer.h"
#include "DeviceResources.h"

class QuestManager;
class CustomShader;
class DeviceResources;

//スライドアクションの列挙数
enum SLIDE_ACTION : int
{
	WAIT = 0,
	SLIDE_OUT,
	COOL_TIME,
	SLIDE_IN
};

class QuestRenderer
{
	// --------------------
	// 固定値
	// --------------------
public:
	// テクスチャサイズの大きさ
	static constexpr int WIDTH = 400;
	static constexpr int HEIGHT = 120;

	// テクスチャの初期座標
	static constexpr int INIT_POSITION_X = 1280;
	static constexpr int INIT_POSITION_Y = 300;

	// 画面サイズの大きさ
	static constexpr float WINDOW_WIDTH = 1280;
	static constexpr float WINDOW_HEIGHT = 720;

	// テクスチャの最終位置
	static constexpr float FINAL_POSITION_WIDTH = WINDOW_WIDTH - WIDTH;

	// アニメーション時間
	static constexpr float ANIMATION_TIME = 1.0f;
	static constexpr float FIER_TIME = 2.0f;
	static constexpr float INITIAL_TIME = -0.5f;
	static constexpr float ANIMATION_WAIT = 0.5f;

	// 透明度
	static constexpr float MAX_ALPHA = 1.0f;

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
		float 							dissolve;   // ディゾルブ
		DirectX::SimpleMath::Vector2	padding;	// 空白
	};

	// --------------------
	// アクセサ
	// --------------------
public:
	void ChangeTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture) { m_texture = texture; }	// テクスチャの変更

	void IsClear(bool flag) { m_clearFlag = flag; }	// クリアフラグの変更
	bool CanChangeQuest() { return m_canChanegQuest; }	// クエストの変更が可能か

	// --------------------
	// 公開関数
	// --------------------
public:
	// コンストラクタ
	QuestRenderer(QuestManager* manager);
	// デストラクタ
	~QuestRenderer();
	// 初期化処理
	void Initialize(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>);
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Draw();
	// 終了処理
	void Finalize();

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

	// 画像をスライドさせない
	void NoSlideTexture();
	// 画像をスライドアウトさせる
	void SlideOutTexture();
	// クールタイム
	void SlideCoolTime();
	// 画像をスライドインさせる
	void SlideInTexture();

	// スライドアクションの追加
	void AddSlideAction();

	// --------------------
	// メンバー変数
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
	// ディゾルブ
	float m_dissolve;

	float	m_currentTime;	// 経過時間
	float	m_elapsedTime;	// 経過時間

	bool m_clearFlag;		// クエストのクリアフラグ
	bool m_canChanegQuest;	// クエストの変更が可能か

	std::vector<std::function<void()>> m_slideActions;	// スライドアクション
	std::function<void()> m_currentAction; // 現在のアクション

	// シェーダー
	std::unique_ptr<CustomShader> m_shader;
	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// ディゾルブパターン画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_dissolveTexture;
	// ステート
	std::unique_ptr<DirectX::CommonStates>	m_states;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
};