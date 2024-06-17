/*
	@file	PlayScene.cpp
	@brief	�v���C�V�[���N���X
*/
#include "pch.h"
#include "PlayScene.h"

#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugCamera.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/GridFloor.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"

#include <Model.h>
#include <cassert>

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"

//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
PlayScene::PlayScene()
	:
	m_commonResources{}
	,m_debugCamera{}
	,m_gridFloor{}
	,m_projection{}
	,m_isChangeScene{}
	,m_player{}
	,m_skySphere{}
	,m_particles{}
{
	m_commonResources = CommonResources::GetInstance();
}

//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
PlayScene::~PlayScene()
{
}

//---------------------------------------------------------
// ����������
//---------------------------------------------------------
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
		0.1f, 10000.0f
	);

	// TPS�J�������擾����
	m_camera = std::make_unique<Camera>();

	// �V�[���ύX�t���O������������
	m_isChangeScene = false;

	// �V���̐����ƃ��f���̓ǂݍ���
	m_skySphere = std::make_unique<SkySphere>();
	m_skySphere->LoadSkySphereModel(device);

	// �p�[�e�B�N���̐����Ə�����
	m_particles = std::make_unique<Particle>();
	m_particles->Create();

	// �v���C���[�̐����Ə�����
	m_player = std::make_unique<Player>();
	m_player->Initialize(device, context, states);

	// �S�̐����Ə�����
	m_enemy = std::make_unique<Enemy>();
	m_enemy->Initialize(device, context, states);
}

//---------------------------------------------------------
// �X�V����
//---------------------------------------------------------
void PlayScene::Update(float elapsedTime)
{
	using namespace DirectX;
	UNREFERENCED_PARAMETER(elapsedTime);

	// �v���C���[�̍X�V����
	m_player->Update(m_enemy->GetPosition(), elapsedTime);
	// �G�̍X�V����
	m_enemy->Update(elapsedTime);
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
}


//---------------------------------------------------------
// �`�悷��
//---------------------------------------------------------
void PlayScene::Render()
{
	using namespace DirectX;

	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// �r���[�s����擾����
	const SimpleMath::Matrix& view = m_camera->GetViewMatrix();
	// �V���̕`��
	m_skySphere->DrawSkySphere(context, states, view, m_projection);
	// �p�[�e�B�N���̃r���{�[�h�쐬
	m_particles->CreateBillboard(m_camera->GetTargetPosition(), m_camera->GetEyePosition(), DirectX::SimpleMath::Vector3::Up);
	// �p�[�e�B�N���̕`��
	m_particles->Render(view, m_projection);
	// �i�q����`�悷��
	m_gridFloor->Render(context, view, m_projection);
	// �v���C���[�̕`����s��
	m_player->Render(context, states, view, m_projection, m_commonResources);
	// �G�̕`����s��
	m_enemy->Render(context, states, view, m_projection);


	// �f�o�b�O�����uDebugString�v�ŕ\������
	auto debugString = m_commonResources->GetDebugString();
	debugString->AddString("Play Scene");
	debugString->AddString("Play Scene");
	debugString->AddString("Camera Angle : %f", XMConvertToDegrees(m_camera->GetCameraAngle()));
	debugString->AddString("Player Angle : %f", XMConvertToDegrees(m_player->GetAngle()));

}

//---------------------------------------------------------
// ��n������
//---------------------------------------------------------
void PlayScene::Finalize()
{
}

//---------------------------------------------------------
// ���̃V�[��ID���擾����
//---------------------------------------------------------
IScene::SceneID PlayScene::GetNextSceneID() const
{
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
		return IScene::SceneID::TITLE;
	}

	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::NONE;
}
