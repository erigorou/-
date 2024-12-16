#pragma once

#ifndef PLAYSCENE_UI_MANAGER
#define PLAYSCENE_UI_MANAGER

#include "pch.h"

// �O���錾
class IUserInterface;
class PlayScene;
class Player;
class PlayerHPUI;
class EnemyHPUI;
class Warning;
class Operation;

class CommonResources; // CommonResources �̑O���錾

namespace DX {
    class DeviceResources; // DX::DeviceResources �̑O���錾
}

class PlaySceneUIManager
{
public:

	// ====================================================================================
	static const DirectX::SimpleMath::Vector2 PLAYER_HP_POSITION; // �v���C���[��HPUI�̍��W


	// ====================================================================================

    PlaySceneUIManager(PlayScene* playScene);   // �R���X�g���N�^
	~PlaySceneUIManager();                      // �f�X�g���N�^

	void Initialize	();  					// �������֐�
	void CreateUI	();						// UI�̐����֐�
	void Update		(float elapsedTime);	// �X�V�֐�
	void Render		();						// �`��֐�
	void Finalize	();						// �I���֐�

private:
    PlayScene* m_playScene;				// �v���C�V�[��
    DX::DeviceResources* m_pDR;			// �f�o�C�X���\�[�X
    CommonResources* m_commonResources;	// ���ʃ��\�[�X

    std::unique_ptr<PlayerHPUI>	m_playerHP;		// �v���C���[��HP
	std::unique_ptr<EnemyHPUI>	m_enemyHP;		// �G�l�~�[��HP
	std::unique_ptr<Warning>	m_warning;		// �x��UI
	std::unique_ptr<Operation>	m_operation;	// �������UI
};

#endif
