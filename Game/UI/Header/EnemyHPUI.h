#pragma once

#ifndef ENEMY_HP_UI
#define ENEMY_HP_UI

#include "pch.h"
#include "DeviceResources.h"

class HPSystem;

class EnemyHPUI
{
private:
	const wchar_t* ENEMY_HP_TEXTURE = L"Resources/Textures/UI/1px.png";	// 1px�̃e�N�X�`���̃p�X�i�摜�Ƃ��Ĉꉞ�ˁH�j

	static constexpr int MAX_WIDTH = 500;

	// �ォ��Q��
	static constexpr int	TOP_POSITION = 650;
	static constexpr int	BOTTOM_POSITION = 680;
	static constexpr float EASING_TIME = 0.5f;

public:
	// �R���X�g���N�^
	EnemyHPUI(HPSystem* hpSystem);
	// �f�X�g���N�^
	~EnemyHPUI();

	// �������֐�
	void Initialize();
	// �X�V����
	void Update();
	// �`�揈��
	void Render();
	// �I������
	void Finalize();

private:

	// �G�l�~�[��HP�N���X
	HPSystem* m_enemyHPclass;
	// �G�l�~�[��HP
	float m_enemyHP;

	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// HP�̃e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// �w�i�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backTexture;
	// �t���[���e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_frameTexture;

	// �o�ߎ���
	float m_totalTime;
};

#endif