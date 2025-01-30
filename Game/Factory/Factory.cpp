#include "pch.h"
#include "Factory.h"
#include "../Messenger/KeyboardMessenger.h"		// �L�[���̓��b�Z���W���[
#include "../Messenger/EventMessenger.h"		// �C�x���g���b�Z���W���[

#include "../Scene/PlayScene.h"								// �v���C�V�[��
#include "Interface/IObserver.h"							// �I�u�U�[�o�[
#include "../Camera/Camera.h"								// �J����
#include "Effects/Particle.h"								// �p�[�e�B�N��
#include "Game/Sound/Sound.h"								// ��

#include "Libraries/MyLib/SkySphere.h"						// �V��
#include "../Stage/Floor/Floor.h"							// ��
#include "../Stage/Sea/Sea.h"								// �C
#include "../Stage/Wall/Wall.h"								// ��
#include "../Player/Player.h"								// �v���C���[
#include "../Boss/Boss.h"									// �S�i�G�j
#include "../Goblin/Goblin.h"								// �S�u����
#include "../Weapon/Sword/Sword.h"							// �v���C���[�̕���
#include "../Weapon/Cudgel/Cudgel.h"						// �S�i�G�j�̕���

#include "Libraries/MyLib/Collision/CollisionManager.h"		// �����蔻�蓝��
#include "../UI/!PlaySceneUIManager/PlaySceneUIManager.h"	// �v���C�V�[����UI
#include "Game/UI/Header/Warning.h"							// HP�������Ƃ��̌x��UI
#include "../EnemyManager/EnemyManager.h"					// �G�}�l�[�W���[
#include "../Quest/QuestManager.h"							// �N�G�X�g�}�l�[�W���[

// ---------------------------------------------
// �����蔻�蓝���̐����֐�
// ---------------------------------------------
std::unique_ptr<CollisionManager> Factory::CreateCollisionManager()
{
	// �����蔻�蓝����錾����
	std::unique_ptr<CollisionManager> collisionManager;
	collisionManager = std::make_unique<CollisionManager>();
	// �����蔻�蓝���̐ݒ�
	return collisionManager;
}


// ---------------------------------------------
// �J�����̐����֐�
// ---------------------------------------------
std::unique_ptr<Camera> Factory::CreateCamera()
{
	// �J������錾����
	std::unique_ptr<Camera> camera;
	camera = std::make_unique<Camera>();

	// �J�����̐ݒ�
	return camera;
}


// ---------------------------------------------
// �V���̐����֐�
// ---------------------------------------------
std::unique_ptr<SkySphere> Factory::CreateSkySphere()
{
	// �V����錾����
	std::unique_ptr<SkySphere> skySphere;
	skySphere = std::make_unique<SkySphere>();
	// ����������
	skySphere->LoadSkySphereModel();
	// �V���̐ݒ�
	return skySphere;
}

// ---------------------------------------------
// �p�[�e�B�N���̐����֐�
// ---------------------------------------------
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


// ---------------------------------------------
// ���̐����֐�
// ---------------------------------------------
std::unique_ptr<Floor> Factory::CreateFloor()
{
	// ����錾����
	std::unique_ptr<Floor> floor;
	floor = std::make_unique<Floor>();
	// ���̐ݒ�
	return floor;
}


// ---------------------------------------------
// �C�̐����֐�
// ---------------------------------------------
std::unique_ptr<Sea> Factory::CreateSea()
{
	std::unique_ptr<Sea> sea;
	sea = std::make_unique<Sea>();
	return sea;
}


// ---------------------------------------------
// �ǂ̐����֐�
// ---------------------------------------------
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


// ---------------------------------------------
// �v���C���[�̐����֐�
// ---------------------------------------------
std::unique_ptr<Player> Factory::CreatePlayer(PlayScene* playScene)
{
	// �v���C���[��錾����
	std::unique_ptr<Player> player;
	// �v���C���[�𐶐�����
	player = std::make_unique<Player>(playScene);
	// ����������
	player->Initialize();
	// �L�[��o�^
	KeyboardMessenger::Attach(DirectX::Keyboard::X			, player.get(), KeyboardMessenger::KeyPressType::PRESSED);
	KeyboardMessenger::Attach(DirectX::Keyboard::LeftShift	, player.get(), KeyboardMessenger::KeyPressType::PRESSED);
	KeyboardMessenger::Attach(DirectX::Keyboard::Left		, player.get(), KeyboardMessenger::KeyPressType::DOWN);
	KeyboardMessenger::Attach(DirectX::Keyboard::Right		, player.get(), KeyboardMessenger::KeyPressType::DOWN);
	KeyboardMessenger::Attach(DirectX::Keyboard::Up			, player.get(), KeyboardMessenger::KeyPressType::DOWN);
	KeyboardMessenger::Attach(DirectX::Keyboard::Down		, player.get(), KeyboardMessenger::KeyPressType::DOWN);
	KeyboardMessenger::Attach(DirectX::Keyboard::L			, player.get(), KeyboardMessenger::KeyPressType::PRESSED);
	return player;
}



// ---------------------------------------------
// �S�i�G�j�̐����֐�
// ---------------------------------------------
std::unique_ptr<Boss> Factory::CreateBoss(PlayScene* playScene)
{
	// �S�i�G�j��錾����
	std::unique_ptr<Boss> enemy;
	enemy = std::make_unique<Boss>(playScene);
	// ����������
	enemy->Initialize();

	EventMessenger::Attach("canHit", std::bind(&Boss::CanHitSword, enemy.get()));
	EventMessenger::Attach("canNotHit",std::bind(&Boss::CanNotHitSword, enemy.get()));

	// �S�i�G�j�̐ݒ�
	return enemy;
}


// ---------------------------------------------
// �S�u�����̐����֐�
// ---------------------------------------------
std::unique_ptr<Goblin> Factory::CreateGoblin(PlayScene* playScene)
{
	// �S�u�����̐錾
	std::unique_ptr<Goblin> goblin;
	goblin = std::make_unique<Goblin>(playScene);
	// ����������
	goblin->Initialize();
	// �S�u�����̐ݒ�
	return goblin;
}



// ---------------------------------------------
// �v���C���[�̕���̐����֐�
// ---------------------------------------------
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


// ---------------------------------------------
// �S�i�G�j�̕���̐����֐�
// ---------------------------------------------
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


// ---------------------------------------------
// �v���C�V�[����UI�̐����֐�
// ---------------------------------------------
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


// ---------------------------------------------
// �G�}�l�[�W���[�̐����֐�
// ---------------------------------------------
std::unique_ptr<EnemyManager> Factory::CreateEnemyManager(PlayScene* playScene)
{
	// �G�}�l�[�W���[��錾����
	std::unique_ptr<EnemyManager> enemyManager;
	enemyManager = std::make_unique<EnemyManager>(playScene);
	// ����������
	enemyManager->Initialize(playScene);
	// �G�}�l�[�W���[�̐ݒ�
	return enemyManager;
}


// ---------------------------------------------
// �N�G�X�g�}�l�[�W���[�̐����֐�
// ---------------------------------------------
std::unique_ptr<QuestManager> Factory::CreateQuestManager(PlayScene* playScene)
{
	// �N�G�X�g�}�l�[�W���[��錾����
	std::unique_ptr<QuestManager> questManager;
	questManager = std::make_unique<QuestManager>(playScene);
	// ����������
	questManager->InitializeQuest();
	// �N�G�X�g�}�l�[�W���[�̐ݒ�
	return questManager;
}