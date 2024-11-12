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

// �t�@�N�g���[���]�b�g ======================================
#include "Game/Factory/Factory.h"

// �V�X�e���� ================================================
#include "Game/Sound/Sound.h"	// ��

// �I�u�W�F�N�g�֘A�@=========================================
#include "Game/Player/Player.h"			// �v���C���[
#include "Game/Enemy/Enemy.h"			// �S
#include "Game/Weapon/Sword/Sword.h"	// ��
#include "Game/Weapon/Cudgel/Cudgel.h"	// ���_

// �X�e�[�W�֘A ==============================================
#include "Game/Stage/Floor/Floor.h"		// ��
#include "Game/Stage/Wall/Wall.h"		// ��

// UI�֘A�@====================================================
#include "Game/UI/!PlaySceneUIManager/PlaySceneUIManager.h"	// UI�`��֘A

// �����蔻��֘A =============================================
#include "Libraries/MyLib/Collision/CollisionManager.h"	// �����蔻��

// �Q�[���f�[�^ ===============================================
#include "Game/Data/GameData.h"

// Sound�@======================================================
#include "Game/Sound/Sound.h"
#include"Interface/IObserver.h"
#include "Game/Observer/Messenger.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayScene::PlayScene()
	:
	m_commonResources{}
	,m_debugCamera{}
	,m_projection{}
	,m_isChangeScene{}
	,m_player{}
	,m_sword{}
	,m_enemy{}
	,m_cudgel{}
	,m_skySphere{}
	,m_particles{}
{
	m_commonResources = CommonResources::GetInstance();
	GameData::GetInstance()->SetBattleResult(GameData::BATTLE_RESULT::NONE);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayScene::~PlayScene()
{
}

/// <summary>
/// �������֐�
/// </summary>
void PlayScene::Initialize()
{
	using namespace DirectX;

	// �f�o�b�O�J�������쐬����
	RECT rect{ m_commonResources->GetDeviceResources()->GetOutputSize() };

	m_debugCamera = std::make_unique<mylib::DebugCamera>();
	m_debugCamera->Initialize(rect.right, rect.bottom);

	// �ˉe�s����쐬����
	m_projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 100000.0f
	);

	// �V�[���ύX�t���O������������
	m_isChangeScene = false;

	// �I�u�W�F�N�g�̐���
	CreateObjects();
}


/// <summary>
/// factory���]�b�g��p���Đ�������֐�
/// </summary>
void PlayScene::CreateObjects()
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();

	Messenger::Clear();	// ���b�Z���W���[�̃N���A

	m_collisionManager	= Factory::CreateCollisionManager	();			// �p�[�e�B�N��
	m_camera			= Factory::CreateCamera				();			// �J����
	m_skySphere			= Factory::CreateSkySphere			(device);	// �V��	
	m_particles			= Factory::CreateParticle			();			// �p�[�e�B�N��
	m_floor				= Factory::CreateFloor				(device);	// �t���A
	m_wall				= Factory::CreateWall				(this);		// ��
	m_player			= Factory::CreatePlayer				(this);		// �v���C��
	m_sword				= Factory::CreateSword				(this);		// ��
	m_enemy				= Factory::CreateEnemy				(this); 	// �S
	m_cudgel			= Factory::CreateCudgel				(this);		// ���_
	m_uiManager			= Factory::CreateUIManager			(this);		// UI�}�l�[�W��

	// �ώ@�҃��X�g���\�[�g����
	Messenger::SortObserverList();
	// �L�[�͈̓��X�g�𐶐�����
	Messenger::CreateKeyRangeList();

	// �J�������v���C�V�[���Őݒ�
	m_camera->ChangeState(m_camera->GetPlayState());

	// BGM�ύX
	Sound::GetInstance()->ChangeBGM(Sound::BGM_TYPE::PLAY);
}



// ���ׂẴL�[�̉�����Ԃ����o����
inline bool IsKeyPress(DirectX::Keyboard::KeyboardStateTracker& stateTracker)
{
	// ���ׂẴL�[�������ꂽ���ǂ������`�F�b�N
	for (int key = 0; key < 256; key++)
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



/// <summary>
/// �X�V�֐�
/// </summary>
/// <param name="elapsedTime">�t���[�����b</param>
void PlayScene::Update(float elapsedTime)
{
	using namespace DirectX;
	UNREFERENCED_PARAMETER(elapsedTime);

	// �L�[�{�[�h�̏�Ԃ��擾����
	m_keyboardState = DirectX::Keyboard::Get().GetState();
	// �L�[�{�[�h�X�e�[�g�g���b�J�[���X�V����
	m_keyboardStateTracker.Update(m_keyboardState);

	// �L�[�{�[�h����������ꂽ���ǂ����𔻒肷��
	if (IsKeyDown	(m_keyboardState)		)	Messenger::Notify(m_keyboardState		); 
	if (IsKeyPress	(m_keyboardStateTracker))	Messenger::Notify(m_keyboardStateTracker); 

	// UI�̍X�V
	m_uiManager->Update(elapsedTime);

	// �v���C���[�̍X�V����
	m_player->Update(m_enemy->GetPosition(), elapsedTime);
	// �v���C���[�̕���̍X�V����
	m_sword->Update(elapsedTime);

	// �S�̍X�V����
	m_enemy->Update(elapsedTime);
	// �S�̕���̍X�V����
	m_cudgel->Update(elapsedTime);

	// �J�����̉�]�s��̍쐬	�����ɂ̓v���C���[�̉�]�p������
	SimpleMath::Matrix matrix = SimpleMath::Matrix::CreateRotationY( XMConvertToRadians ( m_player->GetAngle() ) );
	// �J�����̍X�V
	m_camera->Update(m_player->GetPosition(), m_enemy->GetPosition(), matrix, elapsedTime);

	// �p�[�e�B�N���̍X�V
	m_particles->Update(
		elapsedTime,
		m_player->GetPosition(),
		m_player->GetVelocity()
	);

	// �Փ˔���̍X�V����
	m_collisionManager->Update();

	// ���s�����߂�
	CheckResult();


#ifdef _DEBUG
#endif // _DEBUG
}


/// <summary>
/// �`�揈��
/// </summary>
void PlayScene::Render()
{
	using namespace DirectX;

	// �r���[�s����擾����
	const SimpleMath::Matrix& view = m_camera->GetViewMatrix();

	m_collisionManager->Render(view, m_projection);

	// ==== �X�e�[�W�̕`�� =========================================================
	// �V���̕`��
	m_skySphere->DrawSkySphere(view, m_projection);
	// ����`�悷��
	m_floor->Render(view, m_projection);
	// �ǂ�`�悷��
	m_wall->Render(view, m_projection);

	// === �I�u�W�F�N�g�̕`�� =======================================================
	// �G�̕`��
	m_enemy->Render(view, m_projection);
	// �G�̕���̕`����s��
	m_cudgel->Render(view, m_projection);
	// �v���C���[�̕`����s��
	m_player->Render(view, m_projection);
	// �v���C���[�̕���̕`����s��
	m_sword->Render(view, m_projection);

	//==== �G�t�F�N�g�n�̕`�� ======================================================
	// �p�[�e�B�N���̃r���{�[�h�쐬
	m_particles->CreateBillboard(m_camera->GetTargetPosition(), m_camera->GetEyePosition(), DirectX::SimpleMath::Vector3::Up);
	// �p�[�e�B�N���̕`��
	m_particles->Render(view, m_projection);

	//==== UI�n�̕`�� ======================================================-------
	m_uiManager->Render();
}

/// <summary>
/// �I������
/// </summary>
void PlayScene::Finalize()
{
}

/// <summary>
/// �J������h�炷
/// </summary>
void PlayScene::SetShakeCamera()
{
	m_camera->SetShake();
}


/// <summary>
/// ���̃V�[��ID�̎擾�֐�
/// </summary>
/// <returns>���U���g or �����Ԃ��Ȃ�</returns>
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


void PlayScene::CheckResult()
{
	auto data = GameData::GetInstance();

	// �G�����S
	if (m_enemy->GetEnemyHP()->GetHP() <= 0)
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