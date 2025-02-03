#pragma once

#ifndef PLAYER_HP_UI
#define PLAYER_HP_UI

#include "pch.h"
#include "DeviceResources.h"

class HPSystem;

class PlayerHPUI
{
private:
	const wchar_t* ENEMY_HP_TEXTURE = L"Resources/Textures/UI/1px.png";	// 1px�̃e�N�X�`���̃p�X�i�摜�Ƃ��Ĉꉞ�ˁH�j

	static constexpr int MAX_WIDTH = 300;

	// �ォ��Q��
	static constexpr int TOP_POSITION = 50;
	static constexpr int BOTTOM_POSITION = 80;

	static constexpr int LEFT_POSITION = 50;

public:
	PlayerHPUI(HPSystem* HPSystem);	// �R���X�g���N�^
	~PlayerHPUI();					// �f�X�g���N�^

	void Initialize(DX::DeviceResources* pDR);	// �������֐�
	void LoadTexture(const wchar_t* path);		// �e�N�X�`���̓ǂݍ���

	void Update();		// �X�V����
	void Render();		// �`�揈��
	void Finalize();	// �I������

private:

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;		// �X�v���C�g�o�b�`
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;	// �e�N�X�`��
	DX::DeviceResources* m_pDR;									// �f�o�C�X���\�[�X

	HPSystem* m_playerHPclass;	// �v���C���[��HP�N���X
	float m_playerHP;				// �v���C���[��HP
};

#endif