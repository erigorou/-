// --------------------------------------------
// 
// 名前:	IScene.h
// 概要:	シーンのインターフェースクラス
// 作成:	池田桜輔
// 
// --------------------------------------------

#pragma once

// 前方宣言
class CommonResources;

/// <summary>
/// シーンのインターフェース
/// </summary>
class IScene
{
public:
	// シーンＩＤ
	enum class SceneID : unsigned int
	{
		NONE,
		TITLE,
		PLAY,
		RESULT,
		END
	};

	// -----------------------------
	// 純粋仮想関数
	// -----------------------------
public:
	// デストラクタ
	virtual ~IScene() = default;
	// 初期化処理
	virtual void Initialize() = 0;
	// 更新処理
	virtual void Update(float elapsedTime) = 0;
	// 描画処理
	virtual void Render() = 0;
	// 終了処理
	virtual void Finalize() = 0;

	// 次のシーンIDを取得する
	virtual SceneID GetNextSceneID() const = 0;
	// 前のシーンIDを取得する
	virtual SceneID GetPrevSceneID() const = 0;
};
