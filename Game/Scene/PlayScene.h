/*
	@file	PlayScene.h
	@brief	�v���C�V�[���N���X
*/
#pragma once
#include "IScene.h"
#include <unordered_map>
#include "Game/Camera/Camera.h"
#include "Interface/IObject.h"

#include "Libraries/MyLib/SkySphere.h"
#include "Effects/Particle.h"

// === �f�[�^�����n ==============
class CommonResources;
class Sound;
class HitStop;
// === �v���C���[�֘A ============
class Player;
class Sword;
class Enemy;
class Cudgel;
class Goblin;
// === �X�e�[�W�֘A =============
class Floor;
class Sea;
class Wall;
// UI�֘A ========================
class PlaySceneUIManager;
// �����蔻��֘A ===============
class CollisionManager;

class PlayScene final :	public IScene

	 
{
// �Œ�l *
public:
	// �L�[�{�[�h�̃L�[�̍ő�l
	static constexpr int MAX_KEY = 256;


// �A�N�Z�T�֐� ===
public:
	Player*		GetPlayer()		{ return m_player.get();}
	Enemy*		GetEnemy()		{ return m_enemy.get(); }
	Sword*		GetSword()		{ return m_sword.get(); }
	Cudgel*		GetCudgel()		{ return m_cudgel.get();}
	Particle*	GetParticle()	{ return m_particles.get(); }	

	CollisionManager* GetCollisionManager() { return m_collisionManager.get(); }

	DirectX::SimpleMath::Vector3 GetTargetPosition() { return m_targetEnemy->GetPosition(); }

// ���ʊ֐� ===
public:
	PlayScene();
	~PlayScene()					override;

	void Initialize()				override;
	void Update(float elapsedTime)	override;
	void Render()					override;
	void Finalize()					override;

	void SetShakeCamera();			// �J������h�炷

// ����J�֐� ===
private:
	void CreateObjects();					// �I�u�W�F�N�g�̐���
	SceneID GetNextSceneID()const;			// ���̃V�[��ID���擾
	void UpdateObjects(float elapsedTime);	// �I�u�W�F�N�g�̍X�V
	void UpdateKeyboard();					// �L�[�{�[�h�̍X�V
	void CheckResult();						// ���s����


// �����ϐ� ===
private:
	// �f�[�^�ɕK�v�ȕ� ============================================
	CommonResources* m_commonResources;					// ���ʃ��\�[�X
	std::unique_ptr<mylib::DebugCamera> m_debugCamera;	// �f�o�b�O�J����
	DirectX::SimpleMath::Matrix			m_projection;	// �v���W�F�N�V�����s��

	bool m_isChangeScene;	// �V�[���J�ڃt���O

	// �V�X�e������ ==========================================================================
	Sound* m_sound;											// ��
	std::unique_ptr<PlaySceneUIManager> m_uiManager;		// UI�}�l�[�W��
	std::unique_ptr<CollisionManager>	m_collisionManager;	// �����蔻��}�l�[�W��
	HitStop*							m_hitStop;			// �q�b�g�X�g�b�v
	// �I�u�W�F�N�g�֘A�̕ϐ� ================================================================
	std::unique_ptr<Camera>		m_camera;		// �J����
	std::unique_ptr<SkySphere>	m_skySphere;	// �V��
	std::unique_ptr<Particle>	m_particles;	// �p�[�e�B�N��
	std::unique_ptr<Player>		m_player;		// �v���C���[
	std::unique_ptr<Sword>		m_sword;		// ��
	std::unique_ptr<Enemy>		m_enemy;		// �S
	std::unique_ptr<Cudgel>		m_cudgel;		// ���_
	std::unique_ptr<Goblin>		m_goblin;		// �S�u����
	// �X�e�[�W�֘A�̕ϐ� ====================================================================
	std::unique_ptr<Floor>	m_floor;			// ��
	std::unique_ptr<Sea>	m_sea;				// �C	
	std::unique_ptr<Wall>	m_wall;				// �ǁi�V���̘g�j


	// �L�[�{�[�h�p�̕ϐ� ====================================================================
	DirectX::Keyboard::State				m_keyboardState;
	DirectX::Keyboard::KeyboardStateTracker m_keyboardStateTracker;

	IObject* m_targetEnemy;		// �^�[�Q�b�g�ƂȂ�G

	// �q�b�g�X�g�b�v�̂�����I�u�W�F�N�g�p�̕ϐ�
	float m_smoothDeltaTime;

};
