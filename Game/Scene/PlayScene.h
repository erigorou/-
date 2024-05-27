/*
	@file	PlayScene.h
	@brief	�v���C�V�[���N���X
*/
#pragma once
#include "IScene.h"
#include <unordered_map>
#include "Game/Camera/Camera.h"

// �O���錾
class CommonResources;
class Player;
class Enemy;

class PlayScene final :
    public IScene
{
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	// �f�o�b�O�J����
	std::unique_ptr<mylib::DebugCamera> m_debugCamera;
	// �i�q��
	std::unique_ptr<mylib::GridFloor> m_gridFloor;
	// �ˉe�s��
	DirectX::SimpleMath::Matrix m_projection;
	// �V�[���`�F���W�t���O
	bool m_isChangeScene;
	// TPS�J����
	std::unique_ptr<Camera> m_camera;


public:
	PlayScene();
	~PlayScene() override;

	void Initialize(CommonResources* resources) override;
	void Update(float elapsedTime)override;
	void Render() override;
	void Finalize() override;

	SceneID GetNextSceneID() const;

private:
	std::unique_ptr<Player> m_player;
	std::unique_ptr<Enemy>  m_enemy;
};
