/*
	@file	PlayScene.h
	@brief	�v���C�V�[���N���X
*/
#pragma once
#include "IScene.h"
#include <unordered_map>
#include "Game/Camera/Camera.h"

#include "Libraries/MyLib/SkySphere.h"
#include "Effects/Particle.h"

// === �f�[�^�����n ==============
class CommonResources;
class BGM_Player;

// === �v���C���[�֘A ============
class Player;
class Sword;
class Enemy;
class Cudgel;

// === �X�e�[�W�֘A =============
class Floor;
class Wall;

// UI�֘A ========================
class PlaySceneUIManager;

// �����蔻��֘A ===============
class CollisionManager;


class PlayScene final :	public IScene
{
public:
	// �󂯓n���p�֐� ===============================================
	Player*		GetPlayer()		{ return m_player.get();}
	Enemy*		GetEnemy()		{ return m_enemy.get(); }
	Sword*		GetSword()		{ return m_sword.get(); }
	Cudgel*		GetCudgel()		{ return m_cudgel.get();}
	Particle*	GetParticle()	{ return m_particles.get(); }	

	// �����蔻�菈���p�֐� ==========================================
	CollisionManager* GetCollisionManager() { return m_collisionManager.get(); }

private:

	// �f�[�^�ɕK�v�ȕ� ============================================
	CommonResources* m_commonResources;					// ���ʃ��\�[�X
	std::unique_ptr<mylib::DebugCamera> m_debugCamera;	// �f�o�b�O�J����
	std::unique_ptr<mylib::GridFloor>	m_gridFloor;	// �O���b�h��
	DirectX::SimpleMath::Matrix			m_projection;	// �v���W�F�N�V�����s��


	bool m_isChangeScene;								// �V�[���J�ڃt���O

	std::unique_ptr<Camera>				m_camera;		// �J����
	std::unique_ptr<SkySphere>			m_skySphere;	// �V��
	std::unique_ptr<Particle>			m_particles;	// �p�[�e�B�N��

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

	// �V�X�e������ ==========================================================================
	std::unique_ptr<BGM_Player> m_bgm;						// BGM�Đ�

	// �I�u�W�F�N�g�֘A�̕ϐ� ================================================================
	std::unique_ptr<Player>	m_player;						// �v���C���[
	std::unique_ptr<Sword>	m_sword;						// ��
	std::unique_ptr<Enemy>  m_enemy;						// �S
	std::unique_ptr<Cudgel> m_cudgel;						// ���_

	// �X�e�[�W�֘A�̕ϐ� ====================================================================
	std::unique_ptr<Floor> m_floor;							// ��
	std::unique_ptr<Wall>  m_wall;							// �ǁi�V���̘g�j
	
	// UI�֘A�̕ϐ� ==========================================================================
	std::unique_ptr<PlaySceneUIManager> m_uiManager;		// UI�}�l�[�W��

	// �����蔻��֘A�̕ϐ� ==================================================================
	std::unique_ptr<CollisionManager> m_collisionManager;	// �����蔻��}�l�[�W��
};
