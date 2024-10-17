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
#include "Libraries/MyLib/GridFloor.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"

// �t�@�N�g���[���]�b�g ======================================
#include "Game/Factory/Factory.h"

// �V�X�e���� ================================================
#include "Game/Sound/BGM_Player.h"	// BGM�Đ�

// �I�u�W�F�N�g�֘A�@=========================================
#include "Game/Player/Player.h"			// �v���C���[
#include "Game/Enemy/Enemy.h"			// �S
#include "Game/Weapon/Sword/Sword.h"	// ��
#include "Game/Weapon/Cudgel/Cudgel.h"	// ���_

// �X�e�[�W�֘A ==============================================
#include "Game/Stage/Floor/Floor.h"		// ��
#include "Game/Stage/Wall/Wall.h"		// ��

// UI�֘A�@====================================================
#include "Game/UI/PlaySceneUIManager/Header/PlaySceneUIManager.h"	// UI�`��֘A

// �����蔻��֘A =============================================
#include "Libraries/MyLib/Collision/CollisionManager.h"	// �����蔻��


#include"Interface/IObserver.h"
#include "Game/Observer/Messenger.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayScene::PlayScene()
	:
	m_commonResources{}
	,m_debugCamera{}
	,m_gridFloor{}
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

	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();

	// �O���b�h�����쐬����
	m_gridFloor = std::make_unique<mylib::GridFloor>(device, context, states);

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

	m_collisionManager	= Factory::CreateCollisionManager();	// �p�[�e�B�N��
	m_bgm				= Factory::CreateBGM_Player();			// BGM
	m_camera			= Factory::CreateCamera();				// �J����
	m_skySphere			= Factory::CreateSkySphere(device);		// �V��	
	m_particles			= Factory::CreateParticle();			// �p�[�e�B�N��
	m_floor				= Factory::CreateFloor(device);			// �t���A
	m_wall				= Factory::CreateWall(this);			// ��
	m_player			= Factory::CreatePlayer(this);			// �v���C��
	m_sword				= Factory::CreateSword(this);			// ��
	m_enemy				= Factory::CreateEnemy(this); 			// �S
	m_cudgel			= Factory::CreateCudgel(this);			// ���_
	m_uiManager			= Factory::CreateUIManager(this);		// UI�}�l�[�W��

	// �ώ@�҃��X�g���\�[�g����
	Messenger::SortObserverList();
	// �L�[�͈̓��X�g�𐶐�����
	Messenger::CreateKeyRangeList();
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
	if (IsKeyDown	(m_keyboardState))			
	{
		Messenger::Notify(m_keyboardState); 
	}


	if (IsKeyPress(m_keyboardStateTracker))	
	{ 
		Messenger::Notify(m_keyboardStateTracker); 
	}

	// BGM�̍Đ�
	m_bgm->Update();

	// UI�̍X�V
	m_uiManager->Update();

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
	m_camera->Update(m_player->GetPosition(), m_enemy->GetPosition(), matrix);

	// �p�[�e�B�N���̍X�V
	m_particles->Update(
		elapsedTime,
		m_player->GetPosition(),
		m_player->GetVelocity()
	);

	// �Փ˔���̍X�V����
	m_collisionManager->Update();

	// HP��0�ȉ��ɂȂ�����Q�[���I��
	if (m_enemy->GetEnemyHP()->GetHP() <= 0 || m_player->GetPlayerHP()->GetHP() <= 0)	m_isChangeScene = true;


#ifdef _DEBUG

#endif // _DEBUG
}


/// <summary>
/// �`�揈��
/// </summary>
void PlayScene::Render()
{
	using namespace DirectX;

	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// �r���[�s����擾����
	const SimpleMath::Matrix& view = m_camera->GetViewMatrix();

	// ==== �X�e�[�W�̕`�� =========================================================
	// �V���̕`��
	m_skySphere->DrawSkySphere(context, states, view, m_projection);
	// �i�q����`�悷��
	m_gridFloor->Render(context, view, m_projection);
	// ����`�悷��
	m_floor->Render(context, view, m_projection);
	// �ǂ�`�悷��
	m_wall->Render(context, states, view, m_projection);

	// === �I�u�W�F�N�g�̕`�� =======================================================
	// �G�̕`��
	m_enemy->Render(device, context, states, view, m_projection);
	// �G�̕���̕`����s��
	m_cudgel->Render(device, context, states, view, m_projection);
	// �v���C���[�̕`����s��
	m_player->Render(device, context, states, view, m_projection, m_commonResources);
	// �v���C���[�̕���̕`����s��
	m_sword->Render(device, context, states, view, m_projection, m_commonResources);

	//==== �G�t�F�N�g�n�̕`�� ======================================================
	// �p�[�e�B�N���̃r���{�[�h�쐬
	m_particles->CreateBillboard(m_camera->GetTargetPosition(), m_camera->GetEyePosition(), DirectX::SimpleMath::Vector3::Up);
	// �p�[�e�B�N���̕`��
	m_particles->Render(states, view, m_projection);

	//==== UI�n�̕`�� ======================================================-------
	m_uiManager->Render();
}

/// <summary>
/// �I������
/// </summary>
void PlayScene::Finalize()
{
	m_bgm->FinalizeFMOD();
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
