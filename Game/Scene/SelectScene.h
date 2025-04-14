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
#include "Screen.h"

// 前方宣言
class CollisionManager;
class CommonResources;
class UserInterface;
class IAction;
class Camera;
// ステージ関連
class Sea;
class Wall;
class Floor;
class SkySphere;
class SelectSceneObject;

class SelectScene final : public IScene, public IObserver
{
	// -----------------------------
	// 固定値
	// -----------------------------
public:
	// ■ﾐ=- EspaceキーのUI -=ﾐ■
	static constexpr DirectX::SimpleMath::Vector2 ESPACE_POSITION = DirectX::SimpleMath::Vector2(100.0f, (Screen::BOTTOM - 20.0f));
	static constexpr DirectX::SimpleMath::Vector2 ESPACE_SIZE = DirectX::SimpleMath::Vector2(0.4f, 0.4f);
	
	// ■ﾐ=- SPACEキーのUI -=ﾐ■
	static constexpr DirectX::SimpleMath::Vector2 SPACE_POSITION = DirectX::SimpleMath::Vector2(Screen::RIGHT - 100.0f, (Screen::BOTTOM - 20.0f));
	static constexpr DirectX::SimpleMath::Vector2 SPACE_SIZE = DirectX::SimpleMath::Vector2(0.4f, 0.4f);

	// ■ﾐ=- ステージセレクトのUI -=ﾐ■
	static constexpr DirectX::SimpleMath::Vector2 LOGO_POSITION = DirectX::SimpleMath::Vector2(Screen::LEFT - 100.0f, Screen::TOP);
	static constexpr DirectX::SimpleMath::Vector2 LOGO_SIZE = DirectX::SimpleMath::Vector2(0.6f, 0.6f);

	// ■ﾐ=-　セレクトステージのUI -=ﾐ■
	static constexpr DirectX::SimpleMath::Vector2 TUTORIAL_POSITION = DirectX::SimpleMath::Vector2(Screen::LEFT + 50.0f, Screen::TOP + 275.0f);
	static constexpr DirectX::SimpleMath::Vector2 BOSS_POSITION = DirectX::SimpleMath::Vector2(Screen::LEFT + 50.0f, Screen::TOP + 425.0f);
	// セレクトステージUIの大きさ
	static constexpr DirectX::SimpleMath::Vector2 SELECT_STAGE_UI_SIZE = DirectX::SimpleMath::Vector2(0.5f, 0.5f);


	// ■ﾐ=-　カメラの固定値　-=ﾐ■
	// 視野角
	static constexpr float FOV = 45.0f;
	// 視錐台（近い）
	static constexpr float NEAR_Z = 0.1f;
	// 視錐台（遠い）
	static constexpr float FAR_Z = 50000.0f;
	// カメラの位置
	static constexpr DirectX::SimpleMath::Vector3 CAMERA_POSITION = { 50.0f, 100.0f, 30.0f };
	// カメラの向き
	static constexpr DirectX::SimpleMath::Vector3 CAMERA_DIRECTION = { 0.0f, 200.0f, 200.0f };

	// ■ﾐ=-　シーンインデックス　-=ﾐ■
	// 最大ステージ
	static constexpr int MAX_STAGE_INDEX = 1;
	// 最小ステージ
	static constexpr int MIN_STAGE_INDEX = 0;


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

	// ユーザーインターフェースの生成
	void CreateUI();
	// ステージの生成
	void CreateStage();
	// ステージの描画
	void RenderStage();
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
	// 衝突判定
	std::unique_ptr<CollisionManager> m_collisionManager;

	// カメラ
	std::unique_ptr<Camera> m_camera;

	// 海
	std::unique_ptr<Sea> m_sea;
	// 壁
	std::unique_ptr<Wall> m_wall;
	// 床
	std::unique_ptr<Floor> m_floor;
	// 空
	std::unique_ptr<SkySphere> m_skySphere;
	// セレクトシーンオブジェクト
	std::unique_ptr<SelectSceneObject> m_selectStageObject;

	// プロジェクション行列
	DirectX::SimpleMath::Matrix m_projection;

	// シーン変更フラグ
	bool m_isChangeScene = false;

	// 選択中のステージ番号
	int m_selectIndex;



	// モデル
	DirectX::Model* m_bossModel;
};