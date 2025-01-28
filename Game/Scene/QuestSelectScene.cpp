/*
	@file	QuestSelectScene.cpp
	@brief	�^�C�g���V�[���N���X
*/
#include "pch.h"
#include "QuestSelectScene.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/DebugString.h"
#include "../Factory/Factory.h"
#include "../Sound/Sound.h"
#include "../Data/GameData.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Libraries/MyLib/Math.h"
#include "Effects/Particle.h"
#include "Game/UI/UIAnchor.h"
#include "Game/UI/SceneUIManager/QuestSelectSceneUIManager.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"

#include "../Camera/Camera.h"
#include "../Stage/Floor/Floor.h"
#include "../Stage/Sea/Sea.h"
#include "../TitleObject/TitleEnemy.h"
#include "Libraries/MyLib/SkySphere.h"

#include "Game/UI/Header/IconUI.h"

#include <cassert>

//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
QuestSelectScene::QuestSelectScene()
	:
	m_totalSeconds{},
	m_isChangeScene{},
	m_selectIndex{},
	m_shakePower{1.0f}
{
	m_commonResources = CommonResources::GetInstance();
}

//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
QuestSelectScene::~QuestSelectScene()
{
	// do nothing.
}

//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void QuestSelectScene::Initialize()
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	// �I�u�W�F�N�g�𐶐�����
	CreateObjects();
	// �ˉe�s��𐶐�����
	CreateProjection();
	// �V�[���ύX�t���O������������
	m_isChangeScene = false;
	// ���y�̕ύX
	Sound::ChangeBGM(Sound::BGM_TYPE::TITLE);
}




//---------------------------------------------------------
// �I�u�W�F�N�g�̐���
//---------------------------------------------------------
void QuestSelectScene::CreateObjects()
{
	m_camera	= Factory::CreateCamera	();
	m_floor		= Factory::CreateFloor	();
	m_sea		= Factory::CreateSea();
	m_skySphere = Factory::CreateSkySphere();

	m_enemy = std::make_unique<TitleEnemy>();
	m_enemy->Initialize();
	m_skySphere->LoadSkySphereModel();

	// �^�C�g���V�[���̃J�����X�e�[�g��ݒ�
	m_camera->ChangeState(m_camera->GetTitleState());

	m_particle = Factory::CreateParticle();

	m_uiManager = std::make_unique<QuestSelectSceneUIManager>();
	m_uiManager->Initialize();
}


//---------------------------------------------------------
// �ˉe�s��̐���
//---------------------------------------------------------
void QuestSelectScene::CreateProjection()
{
	RECT rect{ m_commonResources->GetDeviceResources()->GetOutputSize() };

	// �ˉe�s����쐬����
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		DirectX::XMConvertToRadians(40.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 100000.0f
	);
}


//---------------------------------------------------------
// �X�V����
//---------------------------------------------------------
void QuestSelectScene::Update(float elapsedTime)
{
	m_totalSeconds += elapsedTime;

	// �L�[�{�[�h�X�e�[�g�g���b�J�[���擾����
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();
	// �I�u�W�F�N�g�̍X�V
	UpdateObject(elapsedTime);
	// �p�[�e�B�N���̍X�V
	m_particle->Update(elapsedTime, DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero);
	// �X�e�[�W�I��
	SelectStage(kbTracker.get());
}


//---------------------------------------------------------
// �I�u�W�F�N�g�̍X�V
//---------------------------------------------------------
void QuestSelectScene::UpdateObject(const float elapsedTime)
{
	DirectX::SimpleMath::Vector3 zeroV = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Matrix zeroM = DirectX::SimpleMath::Matrix::Identity;

	// �I�u�W�F�N�g�̍X�V
	m_camera->Shake(elapsedTime);
	m_camera->Update(zeroV, zeroV, zeroM, elapsedTime);
	m_enemy->Update(elapsedTime);

	// UI�̍X�V
	m_uiManager->Update(elapsedTime);
}


//---------------------------------------------------------
// �X�e�[�W�I��
//---------------------------------------------------------
void QuestSelectScene::SelectStage(DirectX::Keyboard::KeyboardStateTracker* keyboard)
{
	// �V�[���ύX���͏������Ȃ�
	if (m_isChangeScene) return;

	// �X�y�[�X�L�[�������ꂽ��
	if (keyboard->pressed.Space)
	{
		if (!m_isChangeScene)
		{
			// �Q�[���f�[�^�ɃX�e�[�W��ݒ�
			auto gameData = GameData::GetInstance();
			gameData->SetSelectStage(m_selectIndex);

			// �V�[���ύX�t���O�𗧂Ă�̂Ɖ��y��炷
			m_isChangeScene = true;
			Sound::PlaySE(Sound::SE_TYPE::SYSTEM_OK);
		}
	}

	// �X�e�[�W�I��
	if (keyboard->pressed.Up	)	m_selectIndex = Math::Clamp(m_selectIndex - 1, MIN_STAGE_INDEX, MAX_STAGE_INDEX);
	if (keyboard->pressed.Down	)	m_selectIndex = Math::Clamp(m_selectIndex + 1, MIN_STAGE_INDEX, MAX_STAGE_INDEX);
}


//---------------------------------------------------------
// �`�悷��
//---------------------------------------------------------
void QuestSelectScene::Render()
{
	auto states		= m_commonResources	->	GetCommonStates		();
	auto view		= m_camera			->	GetViewMatrix		();

	// �I�u�W�F�N�g�̕`��
	m_floor->		Render(view, m_projection);
	m_sea->			Render(view, m_projection);
	m_enemy->		Render(view, m_projection);
	m_skySphere->	DrawSkySphere(view, m_projection);


	// �p�[�e�B�N���̕`��
	m_particle->CreateBillboard(m_camera->GetEyePosition(),DirectX::SimpleMath::Vector3::Zero,m_camera->GetUpVector());
	m_particle->Render(view, m_projection);

	// UI�̕`��
	m_uiManager->Render();

	CleateSpamDust(DirectX::SimpleMath::Vector3::Zero);
}


//---------------------------------------------------------
// ��n������
//---------------------------------------------------------
void QuestSelectScene::Finalize()
{
}


//---------------------------------------------------------
// ���̃V�[��ID���擾����
//---------------------------------------------------------
IScene::SceneID QuestSelectScene::GetNextSceneID() const
{
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
		return IScene::SceneID::PLAY;
	}

	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::NONE;
}



void QuestSelectScene::CleateSpamDust(DirectX::SimpleMath::Vector3 pos)
{
	m_particle->CreateSlamDust(pos);
}