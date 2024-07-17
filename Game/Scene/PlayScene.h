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

// 前方宣言
class CommonResources;
class BGM_Player;

class Player;
class Sword;
class Enemy;
class Cudgel;

class Floor;

class PlayScene final :
    public IScene
{
public:
	Player* GetPlayer() { return m_player.get();}
	Enemy*  GetEnemy()  { return m_enemy.get(); }
	Sword* GetSword()	{ return m_sword.get(); }
	Cudgel* GetCudgel() { return m_cudgel.get();}

private:
	// 共通リソース
	CommonResources* m_commonResources;
	// デバッグカメラ
	std::unique_ptr<mylib::DebugCamera> m_debugCamera;
	// 格子床
	std::unique_ptr<mylib::GridFloor>	m_gridFloor;
	// 射影行列
	DirectX::SimpleMath::Matrix			m_projection;
	// シーンチェンジフラグ
	bool m_isChangeScene;
	// TPSカメラ
	std::unique_ptr<Camera>				m_camera;
	// 天球
	std::unique_ptr<SkySphere>			m_skySphere;
	// パーティクル
	std::unique_ptr<Particle>			m_particles;

public:
	PlayScene();
	~PlayScene() override;

	void Initialize() override;
	void Update(float elapsedTime)override;
	void Render() override;
	void Finalize() override;

	void DrawObjects();
	SceneID GetNextSceneID() const;

private:
	// 効果音再生
	std::unique_ptr<BGM_Player> m_bgm;

private:
	// オブジェクト関連の変数
	std::unique_ptr<Player> m_player;			// プレイヤー
	std::unique_ptr<Sword> m_sword;			// 刀
	std::unique_ptr<Enemy>  m_enemy;		// 鬼
	std::unique_ptr<Cudgel> m_cudgel;		// 金棒

private:
	// ステージ関連の変数
	std::unique_ptr<Floor> m_floor;			// 床
};
