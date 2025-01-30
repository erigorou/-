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
class Boss;
class Cudgel;
class Goblin;
// === �X�e�[�W�֘A =============
class Floor;
class Sea;
class Wall;
// �}�l�[�W���[ ========================
class PlaySceneUIManager;
class CollisionManager;
class EnemyManager;
class QuestManager;


class PlayScene final :	public IScene
{

// --------------------------
// �Œ�l
// --------------------------
public:
	// �L�[�{�[�h�̃L�[�̍ő�l
	static constexpr int MAX_KEY = 256;
	// ����p
	static constexpr float FOV = 45.0f;
	// ������
	static constexpr float NEAR_Z = 0.1f;
	static constexpr float FAR_Z = 50000.0f;



// --------------------------
// �A�N�Z�T
// --------------------------
public:
	Player*		GetPlayer()		{ return m_player.get();}
	Boss*		GetBoss();
	Sword*		GetSword()		{ return m_sword.get(); }
	Cudgel*		GetCudgel()		{ return m_cudgel.get();}
	Particle*	GetParticle()	{ return m_particles.get(); }
	EnemyManager* GetEnemyManager() { return m_enemyManager.get(); }
	PlaySceneUIManager* GetUIManager() { return m_uiManager.get(); }

	// �Փ˔���Ǘ��N���X���擾
	CollisionManager* GetCollisionManager() { return m_collisionManager.get(); }

	// �^�[�Q�b�g�ƂȂ�G�̍��W���擾
	DirectX::SimpleMath::Vector3 GetTargetPosition();

	// �N�G�X�g�}�l�[�W���[���擾
	QuestManager* GetQuestManager() { return m_questManager.get(); }

// --------------------------
// ���J�֐�
// --------------------------
public:
	PlayScene();
	~PlayScene()					override;

	void Initialize()				override;
	void Update(float elapsedTime)	override;
	void Render()					override;
	void Finalize()					override;

	void GameEnd();								// �Q�[���I������


// --------------------------
// �����֐�
// --------------------------
private:
	void CreateObjects();					// �I�u�W�F�N�g�̐���
	SceneID GetNextSceneID()const;			// ���̃V�[��ID���擾

	void UpdateObjects(float elapsedTime);	// �I�u�W�F�N�g�̍X�V
	void UpdateCamera(float elapsedTime);	// �J�����̍X�V
	void UpdateKeyboard();					// �L�[�{�[�h�̍X�V

	// �p�[�e�B�N���̕`��
	void DrawParticle(const DirectX::SimpleMath::Matrix &view, DirectX::SimpleMath::Matrix projection);	

	void CheckResult();						// ���s����
	void GameOverChacker();					// �Q�[���I�[�o�[����

// --------------------------
// �����ϐ�
// --------------------------
private:
	// �f�[�^�֘A
	CommonResources* m_commonResources;					// ���ʃ��\�[�X
	std::unique_ptr<mylib::DebugCamera> m_debugCamera;	// �f�o�b�O�J����
	DirectX::SimpleMath::Matrix			m_projection;	// �v���W�F�N�V�����s��
	bool m_isChangeScene; // �V�[���J�ڃt���O

	// �V�X�e������
	Sound*								m_sound;			// ��
	std::unique_ptr<PlaySceneUIManager> m_uiManager;		// UI�}�l�[�W��
	std::unique_ptr<CollisionManager>	m_collisionManager;	// �����蔻��}�l�[�W��
	std::unique_ptr<EnemyManager>		m_enemyManager;		// �G�}�l�[�W��
	HitStop*							m_hitStop;			// �q�b�g�X�g�b�v
	std::unique_ptr<QuestManager>		m_questManager;		// �N�G�X�g�}�l�[�W��

	// �I�u�W�F�N�g�֘A�̕ϐ�
	std::unique_ptr<Camera>		m_camera;		// �J����
	std::unique_ptr<SkySphere>	m_skySphere;	// �V��
	std::unique_ptr<Particle>	m_particles;	// �p�[�e�B�N��
	std::unique_ptr<Player>		m_player;		// �v���C���[
	std::unique_ptr<Sword>		m_sword;		// ��
	std::unique_ptr<Cudgel>		m_cudgel;		// ���_

	// �X�e�[�W�֘A�̕ϐ�
	std::unique_ptr<Floor>	m_floor;			// ��
	std::unique_ptr<Sea>	m_sea;				// �C	
	std::unique_ptr<Wall>	m_wall;				// �ǁi�V���̘g�j


	// �L�[�{�[�h�p�̕ϐ�
	DirectX::Keyboard::State				m_keyboardState;		// �L�[�{�[�h�̏��
	DirectX::Keyboard::KeyboardStateTracker m_keyboardStateTracker;	// �L�[�{�[�h�̏�ԃg���b�J�[

	// �q�b�g�X�g�b�v�̂�����I�u�W�F�N�g�p�̕ϐ�
	float m_smoothDeltaTime;
};
