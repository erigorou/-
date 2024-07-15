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

// �O���錾
class CommonResources;
class BGM_Player;

class Player;
class Sword;
class Enemy;
class Cudgel;

class PlayScene final :
    public IScene
{
public:
	Player* GetPlayer() { return m_player.get();}
	Enemy*  GetEnemy()  { return m_enemy.get(); }
	Sword* GetSword()	{ return m_sword.get(); }
	Cudgel* GetCudgel() { return m_cudgel.get();}

private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	// �f�o�b�O�J����
	std::unique_ptr<mylib::DebugCamera> m_debugCamera;
	// �i�q��
	std::unique_ptr<mylib::GridFloor>	m_gridFloor;
	// �ˉe�s��
	DirectX::SimpleMath::Matrix			m_projection;
	// �V�[���`�F���W�t���O
	bool m_isChangeScene;
	// TPS�J����
	std::unique_ptr<Camera>				m_camera;
	// �V��
	std::unique_ptr<SkySphere>			m_skySphere;
	// �p�[�e�B�N��
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
	// ���ʉ��Đ�
	std::unique_ptr<BGM_Player> m_bgm;

private:
	std::unique_ptr<Player> m_player;
	std::unique_ptr<Sword> m_sword;

	std::unique_ptr<Enemy>  m_enemy;
	std::unique_ptr<Cudgel> m_cudgel;
};
