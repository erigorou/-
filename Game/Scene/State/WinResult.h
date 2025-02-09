/*
	@file	WinResultScene.h
	@brief	リザルトシーンクラス
*/
#pragma once
#include "../IScene.h"

// 前方宣言
class CommonResources;

class WinResult final : public IScene
{
public:

	// -------------------------
	// メンバ関数(公開)
	// -------------------------
public:
	// コンストラクタ
	WinResult();
	// デストラクタ
	~WinResult() override;
	// 初期化処理
	void Initialize() override;
	// 更新処理
	void Update(float elapsedTime)override;
	// 描画処理	
	void Render() override;
	// 終了処理
	void Finalize() override;
	// 次のシーンIDを取得する
	SceneID GetNextSceneID() const;

	// -------------------------
	// メンバ変数
	// -------------------------
private:
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// テクスチャの中心座標
	DirectX::SimpleMath::Vector2 m_texCenter;
	// シーン遷移フラグ
	bool m_isChangeScene;
};
