/*
	@file	TitleScene.h
	@brief	タイトルシーンクラス
*/
#pragma once
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


class TitleScene final : public IScene
{

public:

	static constexpr float DELAY = 1.5f;		// タイトル画面の遅延時間
	static constexpr float ANIM_END = 1.5f;	// タイトル画面のアニメーション終了時間

	static constexpr float TITLE_LOGO_CENTER_Y = -165.0f;	// タイトルの中心座標 ※bottom基準


public:
	void CleateSpamDust(DirectX::SimpleMath::Vector3 pos);

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

	void SetShakeCamera();			// カメラを揺らす

private:
	// 画像のロード処理
	void LoadTextures();
	// 画像の中心：大きさを取得
	void CalculateTextureCenter();
	// オブジェクトを生成
	void CreateObjects();
	// 画像の描画処理
	void DrawTexture();

	// 共通リソース
	CommonResources* m_commonResources;

	std::unique_ptr<DirectX::SpriteBatch>	m_spriteBatch;	// スプライトバッチ
	DirectX::SimpleMath::Matrix				m_projection;	// プロジェクション行列

	std::unique_ptr<DirectX::SpriteFont>				m_spriteFont;	// スプライトフォント
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texture;		// テクスチャ１
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texture2;		// テクスチャ２

	DirectX::SimpleMath::Vector2 m_texCenter;	// テクスチャの中心座標
	DirectX::SimpleMath::Vector2 m_texCenter1;
	DirectX::SimpleMath::Vector2 m_texCenter2;

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

