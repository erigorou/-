#pragma once

#ifndef PLAYER_HP_UI
#define PLAYER_HP_UI

#include "pch.h"
#include "DeviceResources.h"

class HPSystem;
class CustomShader;

class PlayerHPUI
{
private:
    static constexpr int MAX_WIDTH = 300;

    // �ォ��Q��
    static constexpr int TOP_POSITION = 50;
    static constexpr int BOTTOM_POSITION = 80;

    static constexpr int LEFT_POSITION = 50;

    static constexpr int FRALE_WIDTH = 4;

public:
    // �R���X�g���N�^
    PlayerHPUI(HPSystem* HPSystem);
    // �f�X�g���N�^
    ~PlayerHPUI();
    // �������֐�
    void Initialize();
    // �X�V����
    void Update();
    // �`�揈��
    void Render();
    // �I������
    void Finalize();

private:
    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;          // �X�v���C�g�o�b�`
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;    // �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backTexture;    // �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_frameTexture;    // �e�N�X�`��
    DX::DeviceResources* m_pDR;                                    // �f�o�C�X���\�[�X

    HPSystem* m_playerHPclass;  // �v���C���[��HP�N���X
    float m_playerHP;           // �v���C���[��HP
};

#endif
