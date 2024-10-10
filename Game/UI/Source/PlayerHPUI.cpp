// --------------
//
// PlayerHPUI.cpp
//
// --------------

#include "pch.h"
#include "Game/UI/Header/PlayerHPUI.h"
#include "Game/Player/PlayerHP.h"

// ----------------------------
/// <summary>
/// �R���X�g���N�^
/// </summary>
// ----------------------------
PlayerHPUI::PlayerHPUI(PlayerHP* playerHp)
	: m_playerHPclass(playerHp)
	, m_spriteBatch()
	, m_texture()
	, m_pDR(nullptr)
	, m_position()
	, m_scale()
	, m_isDead(false)
{
}


// ----------------------------
/// <summary>
/// �f�X�g���N�^
/// </summary>
// ----------------------------
PlayerHPUI::~PlayerHPUI()
{
}



// ----------------------------
/// <summary>
/// �������֐�
/// </summary>
/// <param name="position">���W</param>
/// <param name="size">�傫��</param>
// ----------------------------
void PlayerHPUI::Initialize(DX::DeviceResources* pDR, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale)
{
	m_pDR = pDR;
	m_position = position;
	m_scale = scale;
	m_playerHP = m_playerHPclass->GetMaxHP();	// �v���C���[��HP���擾

	m_windowWidth = m_pDR->GetScreenViewport().Width;
	m_windowHeight = m_pDR->GetScreenViewport().Height;

	// �J�n���ɂT�A�C�R���𐶐�����
	for (int i = 0; i < m_playerHP; i++)
	{
		Add(HPUI_FILEPATH, position, scale, ANCHOR::MIDDLE_CENTER);

		m_HPIcons[i]->SetPosition(DirectX::SimpleMath::Vector2(position.x + (50.0f * i), position.y));
	}
}

// ----------------------------
/// <summary>
/// �`�揈��
/// </summary>
// ----------------------------
void PlayerHPUI::Update()
{
}


// ----------------------------
/// <summary>
/// �`�揈��
/// </summary>
// ----------------------------
void PlayerHPUI::Render()
{
	m_playerHP = m_playerHPclass->GetHP();	// �v���C���[��HP���擾

	// �A�C�R���̕`��
	for (int i = 0; i < m_playerHP; i++)
	{
		m_HPIcons[i]->Render();
	}
}


// ----------------------------
/// <summary>
/// HPUI�̃A�C�R����ǉ�����֐�
/// </summary>
/// <param name="path">�t�@�C���p�X</param>
/// <param name="position">���W</param>
/// <param name="scale">�傫��</param>
/// <param name="anchor">�A���J�[</param>
// ----------------------------
void PlayerHPUI::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, ANCHOR anchor)
{
	std::unique_ptr<IconUI> icon = std::make_unique<IconUI>();
	icon->Create(m_pDR, path, position, scale, anchor);
	icon->SetWindowSize(m_windowWidth, m_windowHeight);
	m_HPIcons.push_back(std::move(icon));
}



// ----------------------------
/// <summary>
/// �I������
/// </summary>
// ----------------------------
void PlayerHPUI::Finalize()
{
}


