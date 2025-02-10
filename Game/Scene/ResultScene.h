// --------------------------------------------------
// 
// 名前:	ResultScene.h
// 内容:	リザルトシーンを描画するクラス
// 作成:	池田桜輔
// 
// --------------------------------------------------
#pragma once
// インクルード
#include "IScene.h"
#include "Game/Data/GameData.h"

// 前方宣言
class CommonResources;

/// <summary>
/// リザルトシーン
/// </summary>
class ResultScene final : public IScene
{
	// -----------------------------
	// メンバ関数(公開)
	// -----------------------------
public:
	// コンストラクタ
	ResultScene();
	// デストラクタ
	~ResultScene() override;
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

	// -----------------------------
	// メンバ関数(非公開)
	// -----------------------------

private:
	// テクスチャの作成
	void CreateTextures();

	// -----------------------------
	// メンバ変数
	// -----------------------------

private:
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// スクリーンショット
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_captureTexture;

	// テクスチャの中心座標
	DirectX::SimpleMath::Vector2 m_texCenter;
	// スクショテクスチャの中心座標
	DirectX::SimpleMath::Vector2 m_captureTexCenter;

	// シーン遷移フラグ
	bool m_isChangeScene;
	// ゲームデータ
	GameData* m_gameData;
	// 戦闘結果
	GameData::BATTLE_RESULT m_result;
};
