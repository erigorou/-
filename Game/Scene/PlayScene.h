/*
	@file	PlayScene.h
	@brief	プレイシーンクラス
*/
#pragma once
#include "IScene.h"
#include <unordered_map>
#include "Game/Camera/Camera.h"
#include "Interface/IObject.h"

#include "Libraries/MyLib/SkySphere.h"
#include "Effects/Particle.h"

// === データ処理系 ==============
class CommonResources;
class Sound;
class HitStop;
// === プレイヤー関連 ============
class Player;
class Sword;
class Boss;
class Cudgel;
class Goblin;
// === ステージ関連 =============
class Floor;
class Sea;
class Wall;
// マネージャー ========================
class PlaySceneUIManager;
class CollisionManager;
class EnemyManager;
class QuestManager;


class PlayScene final :	public IScene
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
	Player*		GetPlayer()		{ return m_player.get();}
	Boss*		GetBoss();
	Sword*		GetSword()		{ return m_sword.get(); }
	Cudgel*		GetCudgel()		{ return m_cudgel.get();}
	Particle*	GetParticle()	{ return m_particles.get(); }
	EnemyManager* GetEnemyManager() { return m_enemyManager.get(); }
	PlaySceneUIManager* GetUIManager() { return m_uiManager.get(); }

	// 衝突判定管理クラスを取得
	CollisionManager* GetCollisionManager() { return m_collisionManager.get(); }

	// ターゲットとなる敵の座標を取得
	DirectX::SimpleMath::Vector3 GetTargetPosition();

	// クエストマネージャーを取得
	QuestManager* GetQuestManager() { return m_questManager.get(); }

// --------------------------
// 公開関数
// --------------------------
public:
	PlayScene();
	~PlayScene()					override;

	void Initialize()				override;
	void Update(float elapsedTime)	override;
	void Render()					override;
	void Finalize()					override;

	void GameEnd();								// ゲーム終了処理


// --------------------------
// 内部関数
// --------------------------
private:
	void CreateObjects();					// オブジェクトの生成
	SceneID GetNextSceneID()const;			// 次のシーンIDを取得

	void UpdateObjects(float elapsedTime);	// オブジェクトの更新
	void UpdateCamera(float elapsedTime);	// カメラの更新
	void UpdateKeyboard();					// キーボードの更新

	// パーティクルの描画
	void DrawParticle(const DirectX::SimpleMath::Matrix &view, DirectX::SimpleMath::Matrix projection);	

	void CheckResult();						// 勝敗判定
	void GameOverChacker();					// ゲームオーバー判定

// --------------------------
// 内部変数
// --------------------------
private:
	// データ関連
	CommonResources* m_commonResources;					// 共通リソース
	std::unique_ptr<mylib::DebugCamera> m_debugCamera;	// デバッグカメラ
	DirectX::SimpleMath::Matrix			m_projection;	// プロジェクション行列
	bool m_isChangeScene; // シーン遷移フラグ

	// システム周り
	Sound*								m_sound;			// 音
	std::unique_ptr<PlaySceneUIManager> m_uiManager;		// UIマネージャ
	std::unique_ptr<CollisionManager>	m_collisionManager;	// 当たり判定マネージャ
	std::unique_ptr<EnemyManager>		m_enemyManager;		// 敵マネージャ
	HitStop*							m_hitStop;			// ヒットストップ
	std::unique_ptr<QuestManager>		m_questManager;		// クエストマネージャ

	// オブジェクト関連の変数
	std::unique_ptr<Camera>		m_camera;		// カメラ
	std::unique_ptr<SkySphere>	m_skySphere;	// 天球
	std::unique_ptr<Particle>	m_particles;	// パーティクル
	std::unique_ptr<Player>		m_player;		// プレイヤー
	std::unique_ptr<Sword>		m_sword;		// 刀
	std::unique_ptr<Cudgel>		m_cudgel;		// 金棒

	// ステージ関連の変数
	std::unique_ptr<Floor>	m_floor;			// 床
	std::unique_ptr<Sea>	m_sea;				// 海	
	std::unique_ptr<Wall>	m_wall;				// 壁（天球の枠）


	// キーボード用の変数
	DirectX::Keyboard::State				m_keyboardState;		// キーボードの状態
	DirectX::Keyboard::KeyboardStateTracker m_keyboardStateTracker;	// キーボードの状態トラッカー

	// ヒットストップのかかるオブジェクト用の変数
	float m_smoothDeltaTime;
};
