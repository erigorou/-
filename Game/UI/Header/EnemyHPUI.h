#pragma once

#ifndef ENEMY_HP_UI
#define ENEMY_HP_UI

#include "pch.h"
#include "Libraries/Microsoft/DebugDraw.h"
#include "DeviceResources.h"

class EnemyHP;

class EnemyHPUI
{
private:
	const wchar_t* ENEMY_HP_TEXTURE = L"Resources/Textures/UI/1px.png";	// 1px�̃e�N�X�`���̃p�X�i�摜�Ƃ��Ĉꉞ�ˁH�j

	static constexpr int MAX_WIDTH			= 500;

	static constexpr int TOP_POSITION		= 25;
	static constexpr int BOTTOM_POSITION	= 50;

public:
	EnemyHPUI(EnemyHP* enemyHp);	// �R���X�g���N�^
	~EnemyHPUI();					// �f�X�g���N�^

	void Initialize(DX::DeviceResources* pDR);	// �������֐�
	void LoadTexture(const wchar_t* path);		// �e�N�X�`���̓ǂݍ���

	void Update();		// �X�V����
	void Render();		// �`�揈��
	void Finalize();	// �I������

private:

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;		// �X�v���C�g�o�b�`
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;	// �e�N�X�`��
	DX::DeviceResources* m_pDR;									// �f�o�C�X���\�[�X


	EnemyHP* m_enemyHPclass;	// �G�l�~�[��HP�N���X
	int m_enemyHP;				// �G�l�~�[��HP


};

#endif