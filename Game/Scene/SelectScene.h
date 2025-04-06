// --------------------------------------------------
// 
// 名前:	SelectScene.h
// 内容:	セレクトシーンを描画するクラス
// 作成:	池田桜輔
// 
// --------------------------------------------------
#pragma once
// インクルード
#include "IScene.h"
#include "Interface/IObserver.h"
#include "Game/Data/GameData.h"
#include "Game/UI/UIAnchor.h"

// 前方宣言
class CommonResources;
class UserInterface;
class IAction;

class SelectScene final : public IScene, public IObserver
{
	// -----------------------------
	// 固定値
	// -----------------------------
public:


	// ------------------------------
	// メンバ関数(公開)
	// ------------------------------
public:
	// コンストラクタ
	SelectScene();
	// デストラクタ
	~SelectScene() override;
	// 初期化処理
	void Initialize() override;
	// 更新処理
	void Update(float elapsedTime) override;
	// 描画処理
	void Render() override;
	// 終了処理
	void Finalize() override;
	// 次のシーンIDを取得する
	SceneID GetNextSceneID() const;
	// 前のシーンIDを取得する
	SceneID GetPrevSceneID() const;
	// キー入力（押された瞬間）
	void OnKeyPressed(const DirectX::Keyboard::Keys& key) override;
	// キー入力（押されている間）
	void OnKeyDown(const DirectX::Keyboard::Keys& key) override;

	// ------------------------------
	// メンバ変数(非公開)
	// ------------------------------
private:
	// UIの追加
	void AddUserInterface(
		const std::string& texName,
		const DirectX::SimpleMath::Vector2& position,
		const DirectX::SimpleMath::Vector2& size,
		const ANCHOR& anchor,
		IAction* action
	);

	// ユーザーインターフェースの追加
	void CreateUI();
	// 使用キーボードの登録
	void RegisterKeyboard();
	// 下キーが押されたときの処理
	void OnKeyDownArrowDown();
	// 上キーが押されたときの処理
	void OnKeyDownArrowUp();
	// スペースキーが押されたときの処理
	void OnKeyDownSpace();

	// ------------------------------
	// メンバ変数
	// ------------------------------
private:
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// テクスチャ
	std::unique_ptr<CommonResources> m_commonResources;
	// ユーザーインターフェースリスト
	std::vector<std::unique_ptr<UserInterface>> m_uiList;
	// シーン変更フラグ
	bool m_isChangeScene = false;
	// ゲームデータ
	GameData* m_gameData = nullptr;
};