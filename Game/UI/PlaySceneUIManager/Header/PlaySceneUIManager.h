#pragma once

#ifndef PLAYSCENE_UI_MANAGER
#define PLAYSCENE_UI_MANAGER

#include "pch.h"
#include "Libraries/Microsoft/DebugDraw.h"

// �O���錾
class IUserInterface;
class PlayScene;
class Player;
class PlayerHPUI;
class CommonResources; // CommonResources �̑O���錾
namespace DX {
    class DeviceResources; // DX::DeviceResources �̑O���錾
}

class PlaySceneUIManager
{
public:

    // �R���X�g���N�^
    PlaySceneUIManager(PlayScene* playScene);
    // �f�X�g���N�^
    ~PlaySceneUIManager();
    // �������֐�
    void Initialize();
    // UI�����֐�
    void CreateUI();
    // �X�V�֐�
    void Update();
    // �`��֐�
    void Render();
    // �I���֐�
    void Finalize();

private:
    PlayScene* m_playScene;         // �v���C�V�[��
    DX::DeviceResources* m_pDR;     // �f�o�C�X���\�[�X
    CommonResources* m_commonResources; // ���ʃ��\�[�X

    std::unique_ptr<PlayerHPUI> m_playerHP; // �v���C���[��HP
};

#endif
