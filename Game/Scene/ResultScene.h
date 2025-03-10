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
#include "Game/UI/UIAnchor.h"

// 前方宣言
class CommonResources;
class UserInterface;
class IAction;

/// <summary>
/// リザルトシーン
/// </summary>
class ResultScene final : public IScene
{
	// -----------------------------
	// 固定値
	// -----------------------------
public:
	// タイトルに戻るボタンの座標
	static constexpr DirectX::SimpleMath::Vector2 RETURN_BUTTON_POSITION = DirectX::SimpleMath::Vector2(750.0f, 900.0f);
	// ゲーム終了ボタンの座標
	static constexpr DirectX::SimpleMath::Vector2 EXIT_BUTTON_POSITION = DirectX::SimpleMath::Vector2(1000.0f, 900.0f);
	// ボタンの大きさ
	static constexpr DirectX::SimpleMath::Vector2 BUTTON_SIZE = DirectX::SimpleMath::Vector2(1.0f, 1.0f);

	// スクショを描画する位置
	static constexpr DirectX::SimpleMath::Vector2 CAPTURE_POSITION = DirectX::SimpleMath::Vector2(350.0f, 360.0f);
	// スクショの傾き
	static constexpr float CAPTURE_ROTATION = DirectX::XMConvertToRadians(-20.0f);
	// スクショの大きさ
	static constexpr float CAPTURE_SCALE = 0.5f;

	// 背景を描画する位置
	static constexpr DirectX::SimpleMath::Vector2 BACKGROUND_POSITION = DirectX::SimpleMath::Vector2(640.0f, 360.0f);
	// 背景の傾き
	static constexpr float BACKGROUND_ROTATION = DirectX::XMConvertToRadians(0.0f);
	// 背景の大きさ
	static constexpr float BACKGROUND_SCALE = 1.0f;

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
	// 現在のシーンを取得する
	SceneID GetCurrentSceneID() const override { return SceneID::RESULT; }

	// -----------------------------
	// メンバ関数(非公開)
	// -----------------------------
private:
	// テクスチャの作成
	void CreateTextures();
	// ユーザーインターフェースの追加
	void AddUserInterface(
		const std::string textureName,
		DirectX::SimpleMath::Vector2 position,
		DirectX::SimpleMath::Vector2 scale,
		ANCHOR anchor,
		IAction* action
	);
	// ユーザーインターフェースの追加
	void CreateUI();
	// スクショの描画
	void DrawCaptureTexture();
	// 背景の描画
	void DrawBackground();

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
	// ユーザーインターフェースリスト
	std::vector<std::unique_ptr<UserInterface>> m_uiList;

	// シーン遷移フラグ
	bool m_isChangeScene;
	// ゲームデータ
	GameData* m_gameData;
	// 戦闘結果
	GameData::BATTLE_RESULT m_result;
};
