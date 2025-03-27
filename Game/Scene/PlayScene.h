// ------------------------------------------------------------------------------
// 
// 名前:	PlayScene
// 内容:	プレイシーンのオブジェクトを生成し、更新、描画を行う
// 制作:	池田桜輔
// 
// ------------------------------------------------------------------------------
#pragma once
// インクルード
#include "IScene.h"
#include "Interface/IObserver.h"

// 前方宣言
class CommonResources;
class Sound;
class HitStop;
class Player;
class Sword;
class Boss;
class Cudgel;
class Goblin;
class Floor;
class Sea;
class Wall;
class SkySphere;
class PlaySceneUIManager;
class CollisionManager;
class EnemyManager;
class QuestManager;
class Particle;
class Camera;

/// <summary>
/// プレイシーンを更新・描画するクラス
/// </summary>
class PlayScene final : public IScene, public IObserver
{
	// --------------------------
	// 固定値
	// --------------------------
public:
	// キーボードのキーの最大値
	static constexpr int MAX_KEY = 256;
	// 視野角
	static constexpr float FOV = 45.0f;
	// 視錐台
	static constexpr float NEAR_Z = 0.1f;
	static constexpr float FAR_Z = 50000.0f;

	// --------------------------
	// アクセサ
	// --------------------------
public:
	// プレイヤーを取得
	Player* GetPlayer() { return m_player.get(); }
	// ボス鬼を取得
	Boss* GetBoss();
	// 敵のマネージャーを取得
	EnemyManager* GetEnemyManager() { return m_enemyManager.get(); }
	// クエストマネージャーを取得
	QuestManager* GetQuestManager() { return m_questManager.get(); }
	// スクショを取る
	void TakeCapture() { m_isScreenShot = true; }

	// --------------------------
	// メンバ関数(公開)
	// --------------------------
public:
	// コンストラクタ
	PlayScene();
	// デストラクタ
	~PlayScene() override;
	// 初期化
	void Initialize() override;
	// 更新処理
	void Update(float elapsedTime) override;
	// 描画処理
	void Render() override;
	// 終了処理
	void Finalize() override;
	// リザルトシーンへの遷移
	void ChangeResultScene();
	// キーボードの入力検知
	void OnKeyPressed(const DirectX::Keyboard::Keys& key) override;
	// キーボードの入力検知
	void OnKeyDown(const DirectX::Keyboard::Keys& key) override;

	// --------------------------
	// メンバ関数(非公開)
	// --------------------------
private:
	// オブジェクトの生成
	void CreateObjects();
	// 次のシーンIDを取得
	SceneID GetNextSceneID()const;
	// 前のシーンIDを取得
	SceneID GetPrevSceneID()const;

	// オブジェクトの更新
	void UpdateObjects(float elapsedTime);
	// カメラの更新
	void UpdateCamera(float elapsedTime);
	// キーボードの更新
	void UpdateKeyboard();

	// パーティクルの描画
	void DrawParticle(const DirectX::SimpleMath::Matrix& view, DirectX::SimpleMath::Matrix projection);
	// ゲームオーバー判定
	void GameOverChacker();

	// スクリーンショットを取る
	void TakeScreenShot();

	// --------------------------
	// メンバ変数
	// --------------------------
private:
	// プロジェクション行列
	DirectX::SimpleMath::Matrix m_projection;
	// シーン遷移フラグ
	bool m_isChangeScene;
	// サウンドマネージャー
	Sound* m_sound;
	// UIマネージャー
	std::unique_ptr<PlaySceneUIManager> m_uiManager;
	// 当たり判定マネージャー
	std::unique_ptr<CollisionManager> m_collisionManager;
	// 敵マネージャー
	std::unique_ptr<EnemyManager> m_enemyManager;
	// ヒットストップマネージャー
	HitStop* m_hitStop;
	// クエストマネージャー
	std::unique_ptr<QuestManager> m_questManager;
	// カメラ
	std::unique_ptr<Camera> m_camera;
	// 天球
	std::unique_ptr<SkySphere> m_skySphere;
	// パーティクル
	std::unique_ptr<Particle> m_particles;
	// プレイヤー
	std::unique_ptr<Player> m_player;
	// 床
	std::unique_ptr<Floor> m_floor;
	// 海
	std::unique_ptr<Sea> m_sea;
	// 壁
	std::unique_ptr<Wall> m_wall;
	// ヒットストップのかかるオブジェクト用の変数
	float m_smoothDeltaTime;
	// 型抜き画像のみの描画結果を持つ画像のシェーダーリソースビュー
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_captureSRV;
	// スクショを取るフラグ
	bool m_isScreenShot;
};
