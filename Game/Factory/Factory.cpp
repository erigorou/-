#include "pch.h"
#include "Factory.h"

#include "../Scene/PlayScene.h"									// �v���C�V�[��
#include "Libraries/MyLib/Collision/CollisionManager.h"			// �����蔻�蓝��
#include "../Sound/BGM_Player.h"								// BGM�Đ�����
#include "../Camera/Camera.h"									// �J����
#include "Libraries/MyLib/SkySphere.h"							// �V��
#include "Effects/Particle.h"									// �p�[�e�B�N��
#include "../Stage/Floor/Floor.h"								// ��
#include "../Stage/Wall/Wall.h"									// ��
#include "../Player/Player.h"									// �v���C���[
#include "../Enemy/Enemy.h"										// �S�i�G�j
#include "../Weapon/Sword/Sword.h"								// �v���C���[�̕���
#include "../Weapon/Cudgel/Cudgel.h"							// �S�i�G�j�̕���
#include "../UI/PlaySceneUIManager/Header/PlaySceneUIManager.h"	// �v���C�V�[����UI


// �����蔻�蓝���̐����֐�
std::unique_ptr<CollisionManager> Factory::CreateCollisionManager()
{
	// �����蔻�蓝����錾����
	std::unique_ptr<CollisionManager> collisionManager;
	collisionManager = std::make_unique<CollisionManager>();
	// �����蔻�蓝���̐ݒ�
	return collisionManager;
}


// BGM�Đ������̐����֐�
std::unique_ptr<BGM_Player> Factory::CreateBGM_Player()
{
	// BGM�Đ�������錾����
	std::unique_ptr<BGM_Player> bgm_Player;
	bgm_Player = std::make_unique<BGM_Player>();
	// ����������
	bgm_Player->InitializeFMOD("Resources/Sounds/BGM.ogg");
	// BGM�Đ������̐ݒ�
	return bgm_Player;
}


// �J�����̐����֐�
std::unique_ptr<Camera> Factory::CreateCamera()
{
	// �J������錾����
	std::unique_ptr<Camera> camera;
	camera = std::make_unique<Camera>();
	// �J�����̐ݒ�
	return camera;
}


// �V���̐����֐�
std::unique_ptr<SkySphere> Factory::CreateSkySphere(ID3D11Device1* device)
{
	// �V����錾����
	std::unique_ptr<SkySphere> skySphere;
	skySphere = std::make_unique<SkySphere>();
	// ����������
	skySphere->LoadSkySphereModel(device);
	// �V���̐ݒ�
	return skySphere;
}


// �p�[�e�B�N���̐����֐�
std::unique_ptr<Particle> Factory::CreateParticle()
{
	// �p�[�e�B�N����錾����
	std::unique_ptr<Particle> particle;
	particle = std::make_unique<Particle>();
	// ����������
	particle->Create();
	// �p�[�e�B�N���̐ݒ�
	return particle;
}


// ���̐����֐�
std::unique_ptr<Floor> Factory::CreateFloor(ID3D11Device1* device)
{
	// ����錾����
	std::unique_ptr<Floor> floor;
	floor = std::make_unique<Floor>(device);
	// ����������
	// 
	// ���̐ݒ�
	return floor;
}


// �ǂ̐����֐�
std::unique_ptr<Wall> Factory::CreateWall(PlayScene* playScene)
{
	// �ǂ�錾����
	std::unique_ptr<Wall> wall;
	wall = std::make_unique<Wall>(playScene);
	// ����������
	wall->Initialize();
	// �ǂ̐ݒ�
	return wall;

}


// �v���C���[�̐����֐�
std::unique_ptr<Player> Factory::CreatePlayer(PlayScene* playScene)
{
	// �v���C���[��錾����
	std::unique_ptr<Player> player;
	player = std::make_unique<Player>(playScene);
	// ����������
	player->Initialize();
	// �v���C���[�̐ݒ�
	return player;
}



// �S�i�G�j�̐����֐�
std::unique_ptr<Enemy> Factory::CreateEnemy(PlayScene* playScene)
{
	// �S�i�G�j��錾����
	std::unique_ptr<Enemy> enemy;
	enemy = std::make_unique<Enemy>(playScene);
	// ����������
	enemy->Initialize();
	// �S�i�G�j�̐ݒ�
	return enemy;
}


// �v���C���[�̕���̐����֐�
std::unique_ptr<Sword> Factory::CreateSword(PlayScene* playScene)
{
	// �v���C���[�̕����錾����
	std::unique_ptr<Sword> sword;
	sword = std::make_unique<Sword>(playScene);
	// ����������
	sword->Initialize();
	// �v���C���[�̕���̐ݒ�
	return sword;
}


// �S�i�G�j�̕���̐����֐�
std::unique_ptr<Cudgel> Factory::CreateCudgel(PlayScene* playScene)
{
	// �S�i�G�j�̕����錾����
	std::unique_ptr<Cudgel> cudgel;
	cudgel = std::make_unique<Cudgel>(playScene);
	// ����������
	cudgel->Initialize();
	// �S�i�G�j�̕���̐ݒ�
	return cudgel;
}


// �v���C�V�[����UI�̐����֐�
std::unique_ptr<PlaySceneUIManager> Factory::CreateUIManager(PlayScene* playScene)
{
	// �v���C�V�[����UI��錾����
	std::unique_ptr<PlaySceneUIManager> uiManager;
	uiManager = std::make_unique<PlaySceneUIManager>(playScene);
	// ����������
	uiManager->Initialize();
	// �v���C�V�[����UI�̐ݒ�
	return uiManager;
}