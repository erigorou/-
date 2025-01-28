/*
	@file	QuestSelectScene.h
	@brief	タイトルシーンクラス
*/
#pragma once
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

namespace mylib
{
	class DebugCamera;
	class GridFloor;
}


class QuestSelectScene final : public IScene
{
public:
	static constexpr float DELAY = 1.5f;		// タイトル画面の遅延時間
	static constexpr float ANIM_END = 1.5f;		// タイトル画面のアニメーション終了時間

	static constexpr float STAGE_SELECT_DELAY = 2.5f;	// ステージセレクト画面の遅延時間
	static constexpr float STAGE_SELECT_END	  = 1.0f;	// ステージセレクト画面のアニメーション終了時間

	static constexpr float TITLE_LOGO_CENTER_Y = -165.0f;	// タイトルの中心座標 ※bottom基準

	static constexpr float WINDOW_WIDTH = 1280.0f;	// ウィンドウの幅
	static constexpr float WINDOW_HEIGHT = 720.0f;	// ウィンドウの高さ

	static constexpr int MAX_STAGE_INDEX = 1;
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

public:
	void CleateSpamDust(DirectX::SimpleMath::Vector3 pos);

public:

	// コンストラクタ
	QuestSelectScene();
	// デストラクタ
	~QuestSelectScene() override;

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

private:

	// シェーダーの生成
	void CreateShader(const wchar_t* VS, const wchar_t* PS, const wchar_t* GS);

	// コンスタントバッファの作成
	void CreateConstantBuffer();

	// オブジェクト更新処理
	void UpdateObject(const float elapsedTime);

	//// 定数バッファの更新処理
	//void UpdateConstantBuffer();

	// 描画設定
	void SetRenderState();
	// 画像のロード処理
	void LoadTextures();
	// 画像の中心：大きさを取得
	void CalculateTextureCenters();

	void CalculateTextureCenter(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture, DirectX::SimpleMath::Vector2& texCenter);


	// オブジェクトを生成
	void CreateObjects();
	// 画像の描画処理
	void DrawTexture();

	// ステージの描画
	void DrawStageSelect();

	// ステージ選択
	void SelectStage(DirectX::Keyboard::KeyboardStateTracker* keyboard);

	// 共通リソース
	CommonResources* m_commonResources;

	std::unique_ptr<DirectX::SpriteBatch>	m_spriteBatch;	// スプライトバッチ
	DirectX::SimpleMath::Matrix				m_projection;	// プロジェクション行列

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texture;		// テクスチャ１
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texture2;		// テクスチャ２

	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textureList; // テクスチャリスト

	DirectX::SimpleMath::Vector2 m_texCenter;	// テクスチャの中心座標

	DirectX::SimpleMath::Vector2 m_texCenter1;
	DirectX::SimpleMath::Vector2 m_texCenter2;
	DirectX::SimpleMath::Vector2 m_texCenter3;
	DirectX::SimpleMath::Vector2 m_texCenter4;

	std::vector<std::unique_ptr<CustomShader>> m_shaderList;	// シェーダーリスト
	std::vector<std::unique_ptr<Microsoft::WRL::ComPtr<ID3D10ShaderResourceView>>> m_texList;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;

	// シーンチェンジフラグ
	bool m_isChangeScene;

	// 経過時間を取得
	float m_totalSeconds;

	// オブジェクト達
	std::unique_ptr<Camera>		m_camera;		//　カメラ
	std::unique_ptr<Floor>		m_floor;		// 床
	std::unique_ptr<Sea>		m_sea;			// 海
	std::unique_ptr<TitleEnemy> m_enemy;		// 敵
	std::unique_ptr<SkySphere>	m_skySphere;	// スカイスフィア
	std::unique_ptr<Particle> m_particle;		// パーティクル

	int m_selectIndex;	// 選択中のステージ番号

	// カメラを揺らす力
	float m_shakePower;

	std::unique_ptr<QuestSelectSceneUIManager> m_uiManager;
};