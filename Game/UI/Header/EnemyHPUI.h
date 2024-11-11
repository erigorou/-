#pragma once

#ifndef ENEMY_HP_UI
#define ENEMY_HP_UI

#include "pch.h"
#include "Libraries/Microsoft/DebugDraw.h"
#include "DeviceResources.h"

class HPSystem;

class EnemyHPUI
{
private:
	const wchar_t* ENEMY_HP_TEXTURE = L"Resources/Textures/UI/1px.png";	// 1px�̃e�N�X�`���̃p�X�i�摜�Ƃ��Ĉꉞ�ˁH�j

	static constexpr int MAX_WIDTH			= 500;

	// �ォ��Q��
	static constexpr int	TOP_POSITION	= 650;
	static constexpr int	BOTTOM_POSITION	= 670;
	static constexpr float EASING_TIME		= 0.5f;


public:
	EnemyHPUI(HPSystem* hpSystem);	// �R���X�g���N�^
	~EnemyHPUI();					// �f�X�g���N�^

	void Initialize(DX::DeviceResources* pDR);	// �������֐�
	void LoadTexture(const wchar_t* path);		// �e�N�X�`���̓ǂݍ���

	void Update(float elapsedTime);		// �X�V����
	void Render();						// �`�揈��
	void Finalize();					// �I������

private:

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;		// �X�v���C�g�o�b�`
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;	// �e�N�X�`��
	DX::DeviceResources* m_pDR;									// �f�o�C�X���\�[�X

	HPSystem* m_enemyHPclass;	// �G�l�~�[��HP�N���X
	float m_enemyHP;				// �G�l�~�[��HP

	// �o�ߎ���
	float m_totalTime;


};

#endif