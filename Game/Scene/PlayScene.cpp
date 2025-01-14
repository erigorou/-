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
#include "Game/Observer/Messenger.h"					// ���b�Z���W���[
#include "Game/Quest/QuestManager.h"					// �N�G�X�g�}�l�[�W���[
// �I�u�W�F�N�g�֘A�@=========================================
#include "Game/EnemyManager/EnemyManager.h"	// �G�}�l�[�W���[
#include "Game/Player/Player.h"				// �v���C���[
#include "Game/Enemy/Enemy.h"				// �S
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
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();

	Messenger::Clear();	// ���b�Z���W���[�̃N���A

	m_hitStop = HitStop::GetInstance();

	m_collisionManager	= Factory::CreateCollisionManager	();			// �p�[�e�B�N��
	m_camera			= Factory::CreateCamera				();			// �J����
	m_skySphere			= Factory::CreateSkySphere			(device);	// �V��	
	m_particles			= Factory::CreateParticle			();			// �p�[�e�B�N��
	m_floor				= Factory::CreateFloor				(device);	// �t���A
	m_sea				= Factory::CreateSea				();			// �C
	m_wall				= Factory::CreateWall				(this);		// ��
	m_player			= Factory::CreatePlayer				(this);		// �v���C��
	m_sword				= Factory::CreateSword				(this);		// ��
	m_cudgel			= Factory::CreateCudgel				(this);		// ���_

	// �G�}�l�[�W���[�̐���
	m_enemyManager = std::make_unique<EnemyManager>(this);
	// �G�}�l�[�W���[�̏�����
	m_enemyManager->Initialize(this);

	// UI�}�l�[�W���[�̐���
	m_uiManager = std::make_unique<PlaySceneUIManager>(this);
	// UI�}�l�[�W���[�̏�����
	m_uiManager->Initialize();

	// �N�G�X�g�}�l�[�W���[�̐���
	m_questManager = std::make_unique<QuestManager>(this);
	// �N�G�X�g�}�l�[�W���[�̏�����
	m_questManager->InitializeQuest();

	// �ώ@�҃��X�g���\�[�g����
	Messenger::SortObserverList();
	// �L�[�͈̓��X�g�𐶐�����
	Messenger::CreateKeyRangeList();
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
	if (IsKeyDown(m_keyboardState))	Messenger::Notify(m_keyboardState);
	if (IsKeyPress(m_keyboardStateTracker))	Messenger::Notify(m_keyboardStateTracker);

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
	m_sword			->Update(smoothDeltaTime);
	m_cudgel		->Update(smoothDeltaTime);
	m_enemyManager	->Update(smoothDeltaTime);
	m_questManager	->Update(elapsedTime);

	// �J�����̉�]�s��̍쐬	�����ɂ̓v���C���[�̉�]�p������
	DirectX::SimpleMath::Matrix matrix = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_player->GetAngle()));
	// �J�����̍X�V
	m_camera->Update(m_player->GetPosition(), m_enemyManager->GetPicupEnemyPosition(), matrix, smoothDeltaTime);

	// �p�[�e�B�N���̍X�V
	m_particles->Update(elapsedTime,m_player->GetPosition(),m_player->GetVelocity());

	// �Փ˔���̍X�V����
	m_collisionManager->Update();
}


// --------------------------------
// �`��֐�
// --------------------------------
void PlayScene::Render()
{
	// �r���[�s����擾����
	const DirectX::SimpleMath::Matrix& view = m_camera->GetViewMatrix();

	m_collisionManager->Render(view, m_projection);

	// ==== �X�e�[�W�̕`�� =========================================================
	// �V���̕`��
	m_skySphere->DrawSkySphere(view, m_projection);
	// ����`�悷��
	m_floor->Render(view, m_projection);
	// �C��`�悷��
	m_sea->Render(view, m_projection);
	// �ǂ�`�悷��
	m_wall->Render(view, m_projection);

	// === �I�u�W�F�N�g�̕`�� =======================================================
	// �G�̕���̕`����s��
	m_cudgel->Render(view, m_projection);
	// �v���C���[�̕`����s��
	m_player->Render(view, m_projection);
	// �v���C���[�̕���̕`����s��
	m_sword->Render(view, m_projection);

	// �G�̕`��
	m_enemyManager->Render(view, m_projection);

	//==== �G�t�F�N�g�n�̕`�� ======================================================
	// �p�[�e�B�N���̃r���{�[�h�쐬
	m_particles->CreateBillboard(m_camera->GetTargetPosition(), m_camera->GetEyePosition(), DirectX::SimpleMath::Vector3::Up);
	// �p�[�e�B�N���̕`��
	m_particles->Render(view, m_projection);

	m_questManager->DrawQuest();

	//==== UI�n�̕`�� ======================================================-------
	m_uiManager->Render();

}

// --------------------------------
// �I���֐�
// --------------------------------
void PlayScene::Finalize()
{
}

// --------------------------------
// �J������h�炷�֐�
// --------------------------------
void PlayScene::SetShakeCamera(float power)
{
	m_camera->SetShake(power);
}


// --------------------------------
// �{�X�̃|�C���^���擾
// --------------------------------
Enemy* PlayScene::GetEnemy()
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
		return IScene::SceneID::RESULT;
	}

	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::NONE;
}


// --------------------------------
// ���U���g�ɍs���邩�ǂ����𔻒�
// --------------------------------
void PlayScene::CheckResult()
{
	auto data = GameData::GetInstance();

	// �{�X�̃|�C���^���擾
	auto enemy = m_enemyManager->GetBossEnemy();

	// �G�����S
	if (enemy->GetEnemyHP()->GetHP() <= 0)
	{
		m_isChangeScene = true;
		data->SetBattleResult(GameData::BATTLE_RESULT::WIN);
	}

	// �v���C���[�����S
	else if (m_player->GetPlayerHP()->GetHP() <= 0)
	{
		m_isChangeScene = true;
		data->SetBattleResult(GameData::BATTLE_RESULT::LOSE);
	}


#ifdef _DEBUG
	if (m_keyboardStateTracker.IsKeyPressed(DirectX::Keyboard::Keys::F7))
	{
		m_isChangeScene = true;
		data->SetBattleResult(GameData::BATTLE_RESULT::WIN);
	}
#endif // !_DEBUG

}


// --------------------------------
// �Q�[���I������
// --------------------------------
void PlayScene::GameEnd()
{
	CheckResult();
}
