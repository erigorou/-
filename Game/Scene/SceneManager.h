/*
	@file	SceneManager.h
	@brief	シーンマネージャクラス
*/
#pragma once
#include "IScene.h"

// 前方宣言
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

	IScene::SceneID m_nextSceneID;		// 次のシーンID
	bool			m_canChangeScene;	// シーン変更可能かどうか
	bool			m_isFade;			// フェード中かどうか
};
