/*
	@file	SceneManager.cpp
	@brief	�V�[���}�l�[�W���N���X
*/
#include "pch.h"
#include "SceneManager.h"
#include "Game/UI/Fade/Fade.h"

#include "TitleScene.h"
#include "PlayScene.h"
#include "QuestSelectScene.h"
#include "ResultScene.h"

#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include "../Sound/Sound.h"
#include <cassert>


//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
SceneManager::SceneManager()
	: m_currentScene{}
	, m_commonResources{}
	, m_nextSceneID(IScene::SceneID::NONE)
	, m_canChangeScene(false)
	, m_isFade(false)
{
	m_commonResources	= CommonResources::	GetInstance();
	m_sound				= Sound::			GetInstance();
}

//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
SceneManager::~SceneManager()
{
	Finalize();
}

//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void SceneManager::Initialize()
{
	m_fade = std::make_unique<Fade>(this);
	m_fade->Initialize();

#ifndef _DEBUG
	ChangeScene(IScene::SceneID::TITLE);
#endif // !_DEBUG
#ifdef _DEBUG
	ChangeScene(IScene::SceneID::QUEST);
#endif // !_DEBUG

}

//---------------------------------------------------------
// �X�V����
//---------------------------------------------------------
void SceneManager::Update(float elapsedTime)
{
	// �b���̕ۑ�
	GameData::GetInstance()->SetElapsedTime(elapsedTime);
	// ���ʉ��̍Đ�
	Sound::Update();
	// �e�V�[���̍X�V
	m_currentScene->Update(elapsedTime);
	// �t�F�[�h�̍X�V
	m_fade->Update(elapsedTime);

	// �����p�ϐ��F���̃V�[��
	const IScene::SceneID nextSceneID = m_currentScene->GetNextSceneID();

	// �V�[����ύX���Ȃ��Ƃ�
	if (nextSceneID == IScene::SceneID::NONE && ! m_isFade) return;

	// ����Z�b�g�A�b�v
	if (m_nextSceneID == IScene::SceneID::NONE)
	{
		m_nextSceneID = nextSceneID;
		m_isFade = true;
		m_fade->StartFadeOut();
		m_canChangeScene = false;
	}

	// �V�[���ύX�t���O��Fade�N���X������������true�ɂ���
	if (m_canChangeScene)
	{
		m_isFade = false;						// �t�F�[�h���I��
		ChangeScene(m_nextSceneID);				// �V�[����ύX
		m_nextSceneID = IScene::SceneID::NONE;	// �V�[��ID��������
		m_canChangeScene = false;				// �V�[���ύX���֎~
	}
}

//---------------------------------------------------------
// �`�悷��
//---------------------------------------------------------
void SceneManager::Render()
{
	m_currentScene->Render();

	m_fade->Render();
}

//---------------------------------------------------------
// ��n������
//---------------------------------------------------------
void SceneManager::Finalize()
{
	DeleteScene();
}

//---------------------------------------------------------
// �V�[����ύX����
//---------------------------------------------------------
void SceneManager::ChangeScene(IScene::SceneID sceneID)
{
	DeleteScene();
	CreateScene(sceneID);

	m_fade->StartFadeIn();
}

//---------------------------------------------------------
// �V�[�����쐬����
//---------------------------------------------------------
void SceneManager::CreateScene(IScene::SceneID sceneID)
{
	assert(m_currentScene == nullptr);

	switch (sceneID)
	{
		case IScene::SceneID::TITLE:
			m_currentScene = std::make_unique<TitleScene>();
			break;

		case IScene::SceneID::QUEST:
			m_currentScene = std::make_unique<QuestSelectScene>();
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

	assert(m_currentScene && "SceneManager::CreateScene::���̃V�[������������܂���ł����I");
	m_currentScene->Initialize();
}

//---------------------------------------------------------
// �V�[�����폜����
//---------------------------------------------------------
void SceneManager::DeleteScene()
{
	if (m_currentScene)
	{
		m_currentScene.reset();
	}
}
