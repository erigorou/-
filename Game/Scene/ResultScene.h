// --------------------------------------------------
// 名前:	ResultScene.h
// 内容:	リザルトシーンを描画するクラス
// 作成:	池田桜輔
// --------------------------------------------------
#pragma once
#include "IScene.h"
#include "Game/Data/GameData.h"

// 前方宣言
class CommonResources;
class WinResult;
class LoseResult;

class ResultScene final : public IScene
{
	// 固定値
public:

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
	// オブジェクトの作成
	void CreateObjects();

	// -----------------------------
	// メンバ変数
	// -----------------------------

private:
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// テクスチャの中心座標
	DirectX::SimpleMath::Vector2 m_texCenter;

	// 勝利リザルト
	std::unique_ptr<WinResult>	m_winResult;
	// 敗北リザルト
	std::unique_ptr<LoseResult> m_loseResult;
	// 現在のステート
	IScene* m_currentState;
	// シーン遷移フラグ
	bool m_isChangeScene;
	// ゲームデータ
	GameData* m_gameData;
	// 戦闘結果
	GameData::BATTLE_RESULT m_result;
};
