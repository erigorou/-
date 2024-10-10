#pragma once

#ifndef PLAYER_HP_UI
#define PLAYER_HP_UI

// �v���R���p�C���w�b�_�[
#include "pch.h"
#include "Game/UI/Header/IconUI.h"

class PlayerHP;

// �v���C���[��HP��`�悷��N���X
class PlayerHPUI
{
public:
	PlayerHPUI(PlayerHP* playerHp);	// �R���X�g���N�^
	~PlayerHPUI();					// �f�X�g���N�^
	
	void Initialize(DX::DeviceResources* pDR, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale);	// �������֐�
	void Update();		// �X�V����
	void Render();		// �`�揈��
	void Finalize();	// �I������

	void Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, ANCHOR anchor);	// �A�C�R����ǉ�����֐�
	void Delete(std::vector<std::unique_ptr<IconUI>> ui);

private:
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;		// �X�v���C�g�o�b�`
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;	// �e�N�X�`��
	DX::DeviceResources* m_pDR;

	DirectX::SimpleMath::Vector2 m_position;	// ���W
	DirectX::SimpleMath::Vector2 m_scale;		// �A�C�R����̑傫��

	int m_windowWidth, m_windowHeight;	// �E�B���h�E�̑傫��

	int m_playerHP;	// �v���C���[��HP
	bool m_isDead;	// �v���C���[������ł��邩�ǂ���

	PlayerHP* m_playerHPclass;	// �v���C���[��HP�N���X

	std::vector<std::unique_ptr<IconUI>> m_HPIcons;	// HP�̃A�C�R���i�����j




	const wchar_t* HPUI_FILEPATH = L"Resources/Textures/UI/HPICON.png";	// HP�̃e�N�X�`���̃p�X
};

#endif