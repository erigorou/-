// --------------------------------------------------
// 
// 名前:	SceneManager.h
// 内容:	シーンを管理するクラス
// 作成:	池田桜輔
// 
// --------------------------------------------------
#pragma once
// インクルード
#include "IScene.h"

// 前方宣言
class CommonResources;
class GameResources;
class Sound;
class Fade;

class SceneManager
{
	// -----------------------------
	// メンバ関数(公開)
	// -----------------------------
public:
	// コンストラクタ
	SceneManager();
	// デストラクタ
	~SceneManager();

	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Render();
	// 終了処理
	void Finalize();
	// シーン変更可能にする
	void SetCanChangeScene() { m_canChangeScene = true; }

	// -----------------------------
	// メンバ関数(非公開)
	// -----------------------------
private:
	// シーンを変更する
	void ChangeScene(IScene::SceneID sceneID);
	// シーンを作成する
	void CreateScene(IScene::SceneID sceneID);
	// シーンを削除する
	void DeleteScene();
	// キーボードの入力を更新する
	void UpdateKeyboard();
	// Escapeが押されたときのイベント
	void OnEscapeKeyEvent();

	// -----------------------------
	// メンバ変数
	// -----------------------------
private:
	// 現在のシーン
	std::unique_ptr<IScene> m_currentScene;
	// フェード
	std::unique_ptr<Fade> m_fade;
	// ゲームリソース
	GameResources* m_gameResources;
	// サウンド管理
	Sound* m_sound;

	// 現在のシーンID
	IScene::SceneID m_currentSceneID;
	// 次のシーンID
	IScene::SceneID m_nextSceneID;

	// シーン変更可能かどうか
	bool m_canChangeScene;
	// フェード中かどうか
	bool m_isFade;
	// escキーが押されたかどうか
	bool m_isEscape;


	// キーボードの状態
	DirectX::Keyboard::State m_keyboardState;
	// キーボードの状態トラッカー
	DirectX::Keyboard::KeyboardStateTracker m_keyboardStateTracker;
};
