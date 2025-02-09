// ------------------------------------------------------------------------------
// 名前:	PlayScene
// 内容:	タイトルシーンのオブジェクトを生成し、更新、描画を行う
// 制作:	池田桜輔
// ------------------------------------------------------------------------------
#pragma once
// インクルード
#include "pch.h"
#include "IScene.h"

// 前方宣言
class CommonResources;
class Camera;
class Floor;
class Sea;
class TitleEnemy;
class SkySphere;
class Particle;
class CustomShader;
class IconUI;
class QuestSelectSceneUIManager;
class UIAnchor;

/// <summary>
/// タイトルシーンクラス
/// </summary>
class TitleScene final : public IScene
{	
	// -----------------------------
	// 固定値
	// -----------------------------
public:
	// タイトル画面の遅延時間
	static constexpr float DELAY = 1.5f;
	// タイトル画面のアニメーション終了時間
	static constexpr float ANIM_END = 1.5f;

	// ステージセレクト画面の遅延時間
	static constexpr float STAGE_SELECT_DELAY = 2.5f;
	// ステージセレクト画面のアニメーション終了時間
	static constexpr float STAGE_SELECT_END = 1.0f;
	// タイトルの中心座標 ※bottom基準
	static constexpr float TITLE_LOGO_CENTER_Y = -165.0f;
	// ウィンドウの幅
	static constexpr float WINDOW_WIDTH = 1280.0f;
	// ウィンドウの高さ
	static constexpr float WINDOW_HEIGHT = 720.0f;
	// 最大ステージインデックス
	static constexpr int MAX_STAGE_INDEX = 1;
	// 最小ステージインデックス
	static constexpr int MIN_STAGE_INDEX = 0;

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

	// -----------------------------
	// メンバ関数(公開)
	// -----------------------------

public:
	// コンストラクタ
	TitleScene();
	// デストラクタ
	~TitleScene() override;

	// 初期化処理
	void Initialize() override;
	// 更新処理
	void Update(float elapsedTime)override;
	// 描画処理
	void Render() override;
	// 終了処理
	void Finalize() override;
	// ステート変更処理
	SceneID GetNextSceneID() const;

	// -----------------------------
	// メンバ関数(非公開)
	// -----------------------------
private:
	// 射影行列の生成
	void CreateProjection();
	// オブジェクト更新処理
	void UpdateObject(const float elapsedTime);
	// オブジェクトを生成
	void CreateObjects();
	// ステージ選択
	void SelectStage(DirectX::Keyboard::KeyboardStateTracker* keyboard);

	// -----------------------------
	// メンバ変数
	// -----------------------------
private:
	// プロジェクション行列
	DirectX::SimpleMath::Matrix m_projection;
	// シーンチェンジフラグ
	bool m_isChangeScene;
	// 経過時間を取得
	float m_totalSeconds;

	//　カメラ
	std::unique_ptr<Camera> m_camera;
	// 床
	std::unique_ptr<Floor> m_floor;
	// 海
	std::unique_ptr<Sea> m_sea;
	// 敵
	std::unique_ptr<TitleEnemy> m_enemy;
	// 天球
	std::unique_ptr<SkySphere> m_skySphere;
	// パーティクル
	std::unique_ptr<Particle> m_particle;
	// 選択中のステージ番号
	int m_selectIndex;
	// カメラを揺らす力
	float m_shakePower;
	// UIマネージャー
	std::unique_ptr<QuestSelectSceneUIManager> m_uiManager;
};