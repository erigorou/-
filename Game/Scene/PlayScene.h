/*
	@file	PlayScene.h
	@brief	プレイシーンクラス
*/
#pragma once
#include "IScene.h"
#include <unordered_map>
#include "Game/Camera/Camera.h"

#include "Libraries/MyLib/SkySphere.h"
#include "Effects/Particle.h"

// === データ処理系 ==============
class CommonResources;
class BGM_Player;

// === プレイヤー関連 ============
class Player;
class Sword;
class Enemy;
class Cudgel;

// === ステージ関連 =============
class Floor;
class Wall;

// UI関連 ========================
class PlaySceneUIManager;

// 当たり判定関連 ===============
class CollisionManager;


class PlayScene final :	public IScene
{
public:
	// 受け渡し用関数 ===============================================
	Player*		GetPlayer()		{ return m_player.get();}
	Enemy*		GetEnemy()		{ return m_enemy.get(); }
	Sword*		GetSword()		{ return m_sword.get(); }
	Cudgel*		GetCudgel()		{ return m_cudgel.get();}
	Particle*	GetParticle()	{ return m_particles.get(); }	

	// 当たり判定処理用関数 ==========================================
	CollisionManager* GetCollisionManager() { return m_collisionManager.get(); }

private:

	// データに必要な物 ============================================
	CommonResources* m_commonResources;					// 共通リソース
	std::unique_ptr<mylib::DebugCamera> m_debugCamera;	// デバッグカメラ
	std::unique_ptr<mylib::GridFloor>	m_gridFloor;	// グリッド床
	DirectX::SimpleMath::Matrix			m_projection;	// プロジェクション行列


	bool m_isChangeScene;								// シーン遷移フラグ

	std::unique_ptr<Camera>				m_camera;		// カメラ
	std::unique_ptr<SkySphere>			m_skySphere;	// 天球
	std::unique_ptr<Particle>			m_particles;	// パーティクル

public:
	PlayScene();
	~PlayScene()					override;

	void Initialize()				override;
	void Update(float elapsedTime)	override;
	void Render()					override;
	void Finalize()					override;

private:
	void CreateObjects();

	SceneID GetNextSceneID() const;

	// システム周り ==========================================================================
	std::unique_ptr<BGM_Player> m_bgm;						// BGM再生

	// オブジェクト関連の変数 ================================================================
	std::unique_ptr<Player>	m_player;						// プレイヤー
	std::unique_ptr<Sword>	m_sword;						// 刀
	std::unique_ptr<Enemy>  m_enemy;						// 鬼
	std::unique_ptr<Cudgel> m_cudgel;						// 金棒

	// ステージ関連の変数 ====================================================================
	std::unique_ptr<Floor> m_floor;							// 床
	std::unique_ptr<Wall>  m_wall;							// 壁（天球の枠）
	
	// UI関連の変数 ==========================================================================
	std::unique_ptr<PlaySceneUIManager> m_uiManager;		// UIマネージャ

	// 当たり判定関連の変数 ==================================================================
	std::unique_ptr<CollisionManager> m_collisionManager;	// 当たり判定マネージャ
};
