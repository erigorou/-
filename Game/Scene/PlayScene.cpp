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

// �V�X�e���� ================================================
#include "Game/Sound/BGM_Player.h"	// BGM�Đ�

// �I�u�W�F�N�g�֘A�@=========================================
#include "Game/Player/Player.h"					// �v���C���[
#include "Game/Enemy/Enemy.h"					// �S
#include "Game/Weapon/Sword/Sword.h"			// ��
#include "Game/Weapon/Cudgel/Header/Cudgel.h"	// ���_

// �X�e�[�W�֘A ==============================================
#include "Game/Stage/Floor/Floor.h"				// ��
#include "Game/Stage/Wall/Wall.h"				// ��


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

	// TPS�J�������擾����
	m_camera = std::make_unique<Camera>();

	// �V�[���ύX�t���O������������
	m_isChangeScene = false;

	// =================================================
	m_bgm = std::make_unique<BGM_Player>();
	m_bgm->InitializeFMOD("Resources/Sounds/BGM.ogg");

	// =================================================

	// �V���̐����ƃ��f���̓ǂݍ���
	m_skySphere = std::make_unique<SkySphere>();
	m_skySphere->LoadSkySphereModel(device);

	// �p�[�e�B�N���̐����Ə�����
	m_particles = std::make_unique<Particle>();
	m_particles->Create();

	// �v���C���[�̐����Ə�����
	m_player = std::make_unique<Player>(this);
	m_player->Initialize();

	// �v���C���[�̕���̐����Ə�����
	m_sword = std::make_unique<Sword>(this);
	m_sword->Initialize();

	// �S�̐����Ə�����
	m_enemy = std::make_unique<Enemy>(this);
	m_enemy->Initialize();

	// �S�̕���𐶐��Ə�����
	m_cudgel = std::make_unique<Cudgel>(this);
	m_cudgel->Initialize();

	// ���̍쐬�Ə�����
	m_floor = std::make_unique<Floor>(device);
	// �ǂ̐����Ə�����
	m_wall = std::make_unique<Wall>();
	m_wall->Initialize();
}

/// <summary>
/// �X�V�֐�
/// </summary>
/// <param name="elapsedTime">�t���[�����b</param>
void PlayScene::Update(float elapsedTime)
{
	using namespace DirectX;
	UNREFERENCED_PARAMETER(elapsedTime);

	// BGM�̍Đ�
	m_bgm->Update();

	// �v���C���[�̍X�V����
	m_player->Update(m_enemy->GetPosition(), elapsedTime);
	// �v���C���[�̕���̍X�V����
	m_sword->Update(elapsedTime);

	// �S�̍X�V����
	m_enemy->Update(elapsedTime);
	// �S�̕���̍X�V����
	m_cudgel->Update(elapsedTime);

	// �J�����̉�]�s��̍쐬	�����ɂ̓v���C���[�̉�]�p������
	SimpleMath::Matrix matrix
		= SimpleMath::Matrix::CreateRotationY(
			XMConvertToRadians(m_player->GetAngle()));
	// �J�����̍X�V
	m_camera->Update(m_player->GetPosition(), m_enemy->GetPosition(), matrix);

	// �p�[�e�B�N���̍X�V
	m_particles->Update(
		elapsedTime,
		m_player->GetPosition(),
		m_player->GetVelocity()
	);


	// HP��0�ȉ��ɂȂ�����Q�[���I��
	if (m_enemy->GetEnemyHP()->GetHP() <= 0)
	{
		m_isChangeScene = true;
	}
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
