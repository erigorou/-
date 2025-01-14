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

namespace mylib
{
	class DebugCamera;
	class GridFloor;
}


class QuestSelectScene final : public IScene
{

public:

	static constexpr float DELAY = 1.5f;		// タイトル画面の遅延時間
	static constexpr float ANIM_END = 1.5f;	// タイトル画面のアニメーション終了時間

	static constexpr float STAGE_SELECT_DELAY = 2.0f;	// ステージセレクト画面の遅延時間
	static constexpr float STAGE_SELECT_END = 1.5f;	// ステージセレクト画面のアニメーション終了時間

	static constexpr float TITLE_LOGO_CENTER_Y = -165.0f;	// タイトルの中心座標 ※bottom基準

	static constexpr float WINDOW_WIDTH = 1280.0f;	// ウィンドウの幅
	static constexpr float WINDOW_HEIGHT = 720.0f;	// ウィンドウの高さ

	static constexpr float QUEST_LOTO_1_CENTER_X = -115.0f;	// クエストロゴ１の中心座標 ※bottom基準
	static constexpr float QUEST_LOTO_2_CENTER_X = -165.0f;	// クエストロゴ２の中心座標 ※bottom基準


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

	void SetShakeCamera();			// カメラを揺らす

private:
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
};

