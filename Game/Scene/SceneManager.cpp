// ---------------------------------------------
// 
// ���O:	SceneManager.cpp
// ���e:	�V�[���}�l�[�W���N���X
// �쐬:	�r�c����
// 
// ---------------------------------------------
// �C���N���[�h
#include "pch.h"
#include "SceneManager.h"
#include "Game/UI/Fade/Fade.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "ResultScene.h"
#include "Game/Data/GameData.h"
#include "Game/GameResources.h"
#include "../Sound/Sound.h"

#include "Keyboard.h"

//---------------------------------------------------------
/// <summary>
/// �R���X�g���N�^
/// </summary>
//---------------------------------------------------------
SceneManager::SceneManager()
	: 
	m_currentScene{},
	m_nextSceneID(IScene::SceneID::NONE),
	m_currentNextSceneID(IScene::SceneID::TITLE),
	m_canChangeScene(false),
	m_isFade(false)
{
	m_gameResources = GameResources::GetInstance();
	m_sound = Sound::GetInstance();
}


//---------------------------------------------------------
/// <summary>
/// �f�X�g���N�^
/// </summary>
//---------------------------------------------------------
SceneManager::~SceneManager()
{
	Finalize();
}

//---------------------------------------------------------
/// <summary>
/// ����������
/// </summary>
//---------------------------------------------------------
void SceneManager::Initialize()
{
	// �t�F�[�h���쐬
	m_fade = std::make_unique<Fade>(this);
	m_fade->Initialize();

// �f�o�b�O�Ȃ炱��
#ifdef _DEBUG
	ChangeScene(IScene::SceneID::TITLE);
#endif // !_DEBUG

// �����[�X�Ȃ炱��
#ifndef _DEBUG
	ChangeScene(IScene::SceneID::TITLE);
#endif // !_DEBUG
}

//---------------------------------------------------------
/// <summary>
/// �X�V����
/// </summary>
//---------------------------------------------------------
void SceneManager::Update(float elapsedTime)
{
	// Escape�L�[�̓��͌��m
	CheckEscapeKey();
	// �b���̕ۑ�
	GameData::GetInstance()->SetElapsedTime(elapsedTime);
	// ���ʉ��̍Đ�
	Sound::Update();
	// �e�V�[���̍X�V
	m_currentScene->Update(elapsedTime);
	// �t�F�[�h�̍X�V
	m_fade->Update(elapsedTime);

	// �����p�ϐ��F���̃V�[��
	m_currentNextSceneID = m_currentScene->GetNextSceneID();

	// �V�[����ύX���Ȃ��Ƃ�
	if (m_currentNextSceneID == IScene::SceneID::NONE && !m_isFade) return;

	// ����Z�b�g�A�b�v
	if (m_nextSceneID == IScene::SceneID::NONE)
	{
		m_nextSceneID = m_currentNextSceneID;
		m_isFade = true;
		m_fade->StartFadeOut();
		m_canChangeScene = false;
	}

	// �V�[���ύX�t���O��Fade�N���X������������true�ɂ���
	if (m_canChangeScene)
	{
		// �t�F�[�h���I��
		m_isFade = false;
		// �V�[����ύX
		ChangeScene(m_nextSceneID);
		// �V�[��ID��������
		m_nextSceneID = IScene::SceneID::NONE;
		// �V�[���ύX���֎~
		m_canChangeScene = false;
	}
}

//---------------------------------------------------------
/// <summary>
/// �`�悷��
/// </summary>
//---------------------------------------------------------
void SceneManager::Render()
{
	// �V�[���̕`��
	m_currentScene->Render();
	// �t�F�[�h�̕`��
	m_fade->Render();
}

//---------------------------------------------------------
/// <summary>
/// ��n������
/// </summary>
//---------------------------------------------------------
void SceneManager::Finalize()
{
	// �V�[�����폜
	DeleteScene();
}

//---------------------------------------------------------
/// <summary>
/// �V�[����ύX����
/// </summary>
//---------------------------------------------------------
void SceneManager::ChangeScene(IScene::SceneID sceneID)
{
	// �V�[�����폜
	DeleteScene();
	// �V�[�����쐬
	CreateScene(sceneID);
	// �t�F�[�h�C��
	m_fade->StartFadeIn();
}

//---------------------------------------------------------
/// <summary>
/// �V�[�����쐬����
/// </summary>
//---------------------------------------------------------
void SceneManager::CreateScene(IScene::SceneID sceneID)
{
	assert(m_currentScene == nullptr);

	switch (sceneID)
	{
	case IScene::SceneID::TITLE:
		m_currentScene = std::make_unique<TitleScene>();
		break;

	case IScene::SceneID::PLAY:
		m_currentScene = std::make_unique<PlayScene>();
		break;

	case IScene::SceneID::RESULT:
		m_currentScene = std::make_unique<ResultScene>();
		break;

	default:
		assert(!"SceneManager::CreateScene::�V�[���������݂��܂���I");
		// no break
	}

	m_currentScene->Initialize();
}

//---------------------------------------------------------
/// <summary>
/// �V�[�����폜����
/// </summary>
//---------------------------------------------------------
void SceneManager::DeleteScene()
{
	if (m_currentScene)
	{
		m_currentScene.reset();
	}
}

//---------------------------------------------------------
/// <summary>
/// Escape�L�[�̓��͌��m
/// </summary>
// ---------------------------------------------------------
void SceneManager::CheckEscapeKey()
{
	m_keyboardState = DirectX::Keyboard::Get().GetState();
	m_keyboardTracker.Update(m_keyboardState);

	// ESCAPE�L�[�������ꂽ��
	if (m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::Escape))
	{
		// �e�V�[���ł�ESCAPE�L�[�̋���
		EscapeAction();
	}
}

// ---------------------------------------------------------
/// <summary>
/// �e�V�[���ł�ESCAPE�L�[�̋���
/// </summary>
// ---------------------------------------------------------
void SceneManager::EscapeAction()
{
	// ���݂��^�C�g���V�[���Ȃ�
	if (m_currentScene->GetCurrentSceneID() == IScene::SceneID::TITLE)
	{
		// �Q�[�����I������
		PostQuitMessage(0);
	}

	// ���̃V�[�����^�C�g���ɕύX����
	m_nextSceneID = IScene::SceneID::TITLE;
	// �t�F�[�h�A�E�g�J�n
	m_canChangeScene = true;
}
