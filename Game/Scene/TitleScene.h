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
class TitleEnemy;

namespace mylib
{
	class DebugCamera;
	class GridFloor;
}


class TitleScene final : public IScene
{
private:
	DirectX::SimpleMath::Vector2 TITLE_DIRECTION_CENTER = { 0.0f, -75.0f };

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


	// オブジェクト達
	std::unique_ptr<Camera>		m_camera;	//　カメラ
	std::unique_ptr<Floor>		m_floor;	// 床
	std::unique_ptr<TitleEnemy> m_enemy;	// 敵
};

