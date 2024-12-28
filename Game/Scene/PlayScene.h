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
class Enemy;
class Cudgel;
class Goblin;
// === ステージ関連 =============
class Floor;
class Sea;
class Wall;
// UI関連 ========================
class PlaySceneUIManager;
// 当たり判定関連 ===============
class CollisionManager;

class PlayScene final :	public IScene

	 
{
// 固定値 *
public:
	// キーボードのキーの最大値
	static constexpr int MAX_KEY = 256;


// アクセサ関数 ===
public:
	Player*		GetPlayer()		{ return m_player.get();}
	Enemy*		GetEnemy()		{ return m_enemy.get(); }
	Sword*		GetSword()		{ return m_sword.get(); }
	Cudgel*		GetCudgel()		{ return m_cudgel.get();}
	Particle*	GetParticle()	{ return m_particles.get(); }	

	CollisionManager* GetCollisionManager() { return m_collisionManager.get(); }

	DirectX::SimpleMath::Vector3 GetTargetPosition() { return m_targetEnemy->GetPosition(); }

// 共通関数 ===
public:
	PlayScene();
	~PlayScene()					override;

	void Initialize()				override;
	void Update(float elapsedTime)	override;
	void Render()					override;
	void Finalize()					override;

	void SetShakeCamera();			// カメラを揺らす

// 非公開関数 ===
private:
	void CreateObjects();					// オブジェクトの生成
	SceneID GetNextSceneID()const;			// 次のシーンIDを取得
	void UpdateObjects(float elapsedTime);	// オブジェクトの更新
	void UpdateKeyboard();					// キーボードの更新
	void CheckResult();						// 勝敗判定


// 内部変数 ===
private:
	// データに必要な物 ============================================
	CommonResources* m_commonResources;					// 共通リソース
	std::unique_ptr<mylib::DebugCamera> m_debugCamera;	// デバッグカメラ
	DirectX::SimpleMath::Matrix			m_projection;	// プロジェクション行列

	bool m_isChangeScene;	// シーン遷移フラグ

	// システム周り ==========================================================================
	Sound* m_sound;											// 音
	std::unique_ptr<PlaySceneUIManager> m_uiManager;		// UIマネージャ
	std::unique_ptr<CollisionManager>	m_collisionManager;	// 当たり判定マネージャ
	HitStop*							m_hitStop;			// ヒットストップ
	// オブジェクト関連の変数 ================================================================
	std::unique_ptr<Camera>		m_camera;		// カメラ
	std::unique_ptr<SkySphere>	m_skySphere;	// 天球
	std::unique_ptr<Particle>	m_particles;	// パーティクル
	std::unique_ptr<Player>		m_player;		// プレイヤー
	std::unique_ptr<Sword>		m_sword;		// 刀
	std::unique_ptr<Enemy>		m_enemy;		// 鬼
	std::unique_ptr<Cudgel>		m_cudgel;		// 金棒
	std::unique_ptr<Goblin>		m_goblin;		// ゴブリン
	// ステージ関連の変数 ====================================================================
	std::unique_ptr<Floor>	m_floor;			// 床
	std::unique_ptr<Sea>	m_sea;				// 海	
	std::unique_ptr<Wall>	m_wall;				// 壁（天球の枠）


	// キーボード用の変数 ====================================================================
	DirectX::Keyboard::State				m_keyboardState;
	DirectX::Keyboard::KeyboardStateTracker m_keyboardStateTracker;

	IObject* m_targetEnemy;		// ターゲットとなる敵

	// ヒットストップのかかるオブジェクト用の変数
	float m_smoothDeltaTime;

};
