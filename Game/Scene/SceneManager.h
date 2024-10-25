/*
	@file	SceneManager.h
	@brief	�V�[���}�l�[�W���N���X
*/
#pragma once
#include "IScene.h"

// �O���錾
class CommonResources;
class Sound;
class Fade;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void Initialize();
	void Update(float elapsedTime);
	void Render();
	void Finalize();

	void SetCanChangeScene() { m_canChangeScene = true; }

private:
	void ChangeScene(IScene::SceneID sceneID);
	void CreateScene(IScene::SceneID sceneID);
	void DeleteScene();

	std::unique_ptr<IScene> m_currentScene;
	std::unique_ptr<Fade>	m_fade;
	
	CommonResources*		m_commonResources;
	Sound*					m_sound;

	IScene::SceneID m_nextSceneID;		// ���̃V�[��ID
	bool			m_canChangeScene;	// �V�[���ύX�\���ǂ���
	bool			m_isFade;			// �t�F�[�h�����ǂ���
};
