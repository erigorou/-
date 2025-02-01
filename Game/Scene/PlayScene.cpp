/*
	@file	PlayScene.cpp
	@brief	�v���C�V�[���N���X
*/
#include "pch.h"
#include "PlayScene.h"

#include <Model.h>
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugCamera.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
// �V�X�e���� ================================================
#include "Game/Factory/Factory.h"						// �t�@�N�g��
#include "Game/Sound/Sound.h"							// ��
#include "Game/HitStop/HitStop.h"						// �q�b�g�X�g�b�v
#include "Libraries/MyLib/Collision/CollisionManager.h"	// �����蔻��
#include "Game/Data/GameData.h"							// �Q�[���f�[�^
#include"Interface/IObserver.h"							// �I�u�U�[�o�[
#include "Game/Messenger/KeyboardMessenger.h"			// ���b�Z���W���[
#include "Game/Messenger/EventMessenger.h"				// �C�x���g���b�Z���W���[
#include "Game/Quest/QuestManager.h"					// �N�G�X�g�}�l�[�W���[
// �I�u�W�F�N�g�֘A�@=========================================
#include "Game/EnemyManager/EnemyManager.h"	// �G�}�l�[�W���[
#include "Game/Player/Player.h"				// �v���C���[
#include "Game/Boss/Boss.h"					// �S
#include "Game/Weapon/Sword/Sword.h"		// ��
#include "Game/Weapon/Cudgel/Cudgel.h"		// ���_
#include "Game/Goblin/Goblin.h"				// �S�u����

#include "Game/Stage/Floor/Floor.h"			// ��
#include "Game/Stage/Sea/Sea.h"				// �C
#include "Game/Stage/Wall/Wall.h"			// ��
// UI�֘A�@====================================================
#include "Game/UI/!PlaySceneUIManager/PlaySceneUIManager.h"	// UI�`��֘A


// ----------------
// �R���X�g���N�^
// ----------------
PlayScene::PlayScene()
	: m_commonResources{}
	, m_debugCamera{}
	, m_projection{}
	, m_isChangeScene{false}
	, m_smoothDeltaTime{}
{
	m_commonResources = CommonResources::GetInstance();
	GameData::GetInstance()->SetBattleResult(GameData::BATTLE_RESULT::NONE);
}


// ----------------
// �f�X�g���N�^
// ----------------
PlayScene::~PlayScene()
{
}


// ----------------
// �������֐�
// ----------------
void PlayScene::Initialize()
{
	// �f�o�b�O�J�������쐬����
	RECT rect{ m_commonResources->GetDeviceResources()->GetOutputSize() };

	m_debugCamera = std::make_unique<mylib::DebugCamera>();
	m_debugCamera->Initialize(rect.right, rect.bottom);

	// �ˉe�s����쐬����
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		DirectX::XMConvertToRadians(FOV),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		NEAR_Z, FAR_Z);

	// �I�u�W�F�N�g�̐���
	CreateObjects();
}


// ----------------
// �I�u�W�F�N�g�̐���
// ----------------
void PlayScene::CreateObjects()
{
	KeyboardMessenger::Clear();	// ���b�Z���W���[�̃N���A

	m_hitStop = HitStop::GetInstance();

	m_collisionManager	= Factory::CreateCollisionManager	();	// �p�[�e�B�N��
	m_camera			= Factory::CreateCamera				();	// �J����
	m_particles			= Factory::CreateParticle			();	// �p�[�e�B�N��

	m_skySphere			= Factory::CreateSkySphere			();	// �V��	
	m_floor				= Factory::CreateFloor				();	// �t���A
	m_sea				= Factory::CreateSea				();	// �C
	m_wall				= Factory::CreateWall				();	// ��
	m_player			= Factory::CreatePlayer				(this);	// �v���C��

	m_uiManager		= Factory::CreateUIManager				(this);	// UI�}�l�[�W���[
	m_enemyManager	= Factory::CreateEnemyManager			(this);	// �G�}�l�[�W���[
	m_questManager	= Factory::CreateQuestManager			(this);	// �N�G�X�g�}�l�[�W���[


	m_uiManager->CreateUI();	// UI�̐���
	// �ώ@�҃��X�g���\�[�g����
	KeyboardMessenger::SortObserverList();
	// �L�[�͈̓��X�g�𐶐�����
	KeyboardMessenger::CreateKeyRangeList();
	// �J�������v���C�V�[���Őݒ�
	m_camera->ChangeState(m_camera->GetPlayState());
	// BGM�ύX
	Sound::GetInstance()->ChangeBGM(Sound::BGM_TYPE::PLAY);
}



// --------------------------------
// �L�[�������ꂽ���ǂ����𔻒肷��
// --------------------------------
inline bool IsKeyPress(DirectX::Keyboard::KeyboardStateTracker& stateTracker)
{
	// ���ׂẴL�[�������ꂽ���ǂ������`�F�b�N
	for (int key = 0; key < PlayScene::MAX_KEY; key++)
	{
		// ����̃L�[��������Ă��邩���m�F
		if (stateTracker.IsKeyPressed(static_cast<DirectX::Keyboard::Keys>(key)))
		{
			return true; // �����ꂽ�L�[������� true ��Ԃ�
		}
	}
	// �ǂ̃L�[��������Ă��Ȃ��ꍇ
	return false;
}


// --------------------------------
// �L�[����������ꂽ���ǂ����𔻒肷��
// --------------------------------
inline bool IsKeyDown(DirectX::Keyboard::State& state)
{
	// �L�[�{�[�h�X�e�[�g�ւ̃|�C���^���擾����
	auto ptr = reinterpret_cast<uint32_t*>(&state);
	for (int key = 0; key < 0xff; key++)
	{
		const unsigned int buffer = 1u << (key & 0x1f);
		// �L�[����������ꂽ���ǂ����𒲂ׂ�
		if (ptr[(key >> 5)] && buffer)	 return true;
	}
	// �L�[�͉��������Ă��Ȃ�
	return false;
}


// --------------------------------
// �X�V����
// --------------------------------
void PlayScene::Update(float elapsedTime)
{
	// �L�[�{�[�h�̍X�V����
	UpdateKeyboard();
	// �I�u�W�F�N�g�̍X�V����
	UpdateObjects(elapsedTime);
}


// --------------------------------
// �L�[�{�[�h�̍X�V����
// --------------------------------
void PlayScene::UpdateKeyboard()
{
	// �L�[�{�[�h�̏�Ԃ��擾����
	m_keyboardState = DirectX::Keyboard::Get().GetState();
	// �L�[�{�[�h�X�e�[�g�g���b�J�[���X�V����
	m_keyboardStateTracker.Update(m_keyboardState);

	// �L�[�{�[�h����������ꂽ���ǂ����𔻒肷��
	if (IsKeyDown(m_keyboardState))			KeyboardMessenger::Notify(m_keyboardState);
	if (IsKeyPress(m_keyboardStateTracker))	KeyboardMessenger::Notify(m_keyboardStateTracker);

	if (m_keyboardStateTracker.IsKeyPressed(DirectX::Keyboard::Keys::Space)) m_enemyManager->ChangeCameraTarget();
}


// --------------------------------
// �I�u�W�F�N�g�̍X�V����
// --------------------------------
void PlayScene::UpdateObjects(float elapsedTime)
{
	// �q�b�g�X�g�b�v�̍X�V
	m_hitStop->Update(elapsedTime);
	// �q�b�g�X�g�b�v�̎c�莞�Ԃ��擾
	float smoothDeltaTime = m_hitStop->GetSmoothDeltaTime();

	// �I�u�W�F�N�g�̍X�V
	m_uiManager		->Update(elapsedTime);
	m_player		->Update(smoothDeltaTime);
	m_enemyManager	->Update(smoothDeltaTime);
	m_questManager	->Update(elapsedTime);
	UpdateCamera(elapsedTime);

	// �p�[�e�B�N���̍X�V
	m_particles->Update(elapsedTime,m_player->GetPosition(),m_player->GetVelocity());

	// �Փ˔���̍X�V����
	m_collisionManager->Update();


	GameOverChacker();	// �Q�[���I�[�o�[����
}


// --------------------------------
// �`��֐�
// --------------------------------
void PlayScene::Render()
{
	// �r���[�s����擾����
	const DirectX::SimpleMath::Matrix& view = m_camera->GetViewMatrix();

	m_collisionManager->Render(view, m_projection);	// �����蔻��

	m_skySphere->	DrawSkySphere	(view, m_projection);	// ��
	m_floor->		Render			(view, m_projection);	// �n��
	m_sea->			Render			(view, m_projection);	// �C
	m_wall->		Render			(view, m_projection);	// ��

	m_player->		Render(view, m_projection);	// �v���C���[
	m_enemyManager->Render(view, m_projection); // �G�i�����j
	
	DrawParticle(view, m_projection);	// �p�[�e�B�N��

	m_questManager->DrawQuest();	// �N�G�X�g
	m_uiManager->Render();			// UI
}

// --------------------------------
// �p�[�e�B�N���̕`��
// --------------------------------
void PlayScene::DrawParticle(const DirectX::SimpleMath::Matrix& view, DirectX::SimpleMath::Matrix projection)
{
	// �r���{�[�h�s��̌v�Z
	m_particles->CreateBillboard(m_camera->GetTargetPosition(), m_camera->GetEyePosition(), DirectX::SimpleMath::Vector3::Up);
	// �p�[�e�B�N���̕`��
	m_particles->Render(view, projection);
}


// --------------------------------
// �I���֐�
// --------------------------------
void PlayScene::Finalize()
{
}

// --------------------------------
// �J�����̍X�V
// --------------------------------
void PlayScene::UpdateCamera(float elapsedTime)
{
	// �J�����̍X�V
	m_camera->Update(m_player->GetPosition(), m_enemyManager->GetPicupEnemyPosition(), elapsedTime);
}

// --------------------------------
// �{�X�̃|�C���^���擾
// --------------------------------
Boss* PlayScene::GetBoss()
{
	return m_enemyManager->GetBossEnemy();
}

// --------------------------------
// �^�[�Q�b�g�ƂȂ�G�̍��W���擾
// --------------------------------
DirectX::SimpleMath::Vector3 PlayScene::GetTargetPosition()
{
	return m_enemyManager->GetPicupEnemyPosition();
}


// --------------------------------
// ���̃V�[��ID���擾
// --------------------------------
IScene::SceneID PlayScene::GetNextSceneID() const
{
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
		return IScene::SceneID::QUEST;
	}

	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::NONE;
}



// --------------------------------
// ���U���g�ɍs���邩�ǂ����𔻒�
// --------------------------------
void PlayScene::CheckResult()
{
	m_isChangeScene = true;
}


// --------------------------------
// �Q�[���I�[�o�[����
// --------------------------------
void PlayScene::GameOverChacker()
{
	static bool flag = true;

	if (m_keyboardStateTracker.IsKeyPressed(DirectX::Keyboard::Keys::F2)) {
		flag = !flag;
	}

	if (flag) return;

	// �v���C���[�����S
	if (m_player->GetPlayerHP()->GetHP() <= 0)
	{
		GameEnd();
	}
}


// --------------------------------
// �Q�[���I������
// --------------------------------
void PlayScene::GameEnd()
{
	CheckResult();
}
