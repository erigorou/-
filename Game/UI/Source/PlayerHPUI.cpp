// --------------
//
// PlayerHPUI.cpp
//
// --------------

#include "pch.h"
#include "Game/UI/Header/PlayerHPUI.h"
#include "Game/Player/PlayerHP.h"
#include "Game/Screen.h"


// ----------------------------
/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="enemyHp">�G��HP����������N���X�̃|�C���^</param>
// ----------------------------
PlayerHPUI::PlayerHPUI(PlayerHP* playerHp)
	: m_playerHPclass(playerHp)
	, m_spriteBatch()
	, m_playerHP(0)
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


void PlayerHPUI::Initialize(DX::DeviceResources* pDR)
{
	m_pDR = pDR;
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(pDR->GetD3DDeviceContext());	// �X�v���C�g�o�b�`�̐ݒ�
	
	LoadTexture(ENEMY_HP_TEXTURE);				// �e�N�X�`���̓ǂݍ���
	m_playerHP = m_playerHPclass->GetMaxHP();	// �v���C���[��HP���擾
}


/// <summary>
/// �e�N�X�`���̓ǂݍ��݊֐�
/// </summary>
/// <param name="path">�t�@�C���p�X</param>
void PlayerHPUI::LoadTexture(const wchar_t* path)
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, m_texture.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to load texture.", NULL, MB_OK);
	}
}



// ----------------------------
/// <summary>
/// �X�V�֐�
/// </summary>
// ----------------------------
void PlayerHPUI::Update()
{
}


// ----------------------------
/// <summary>
/// �`��֐�
/// </summary>
// ----------------------------
// ----------------------------
/// <summary>
/// �`��֐�
/// </summary>
// ----------------------------
void PlayerHPUI::Render()
{
    m_playerHP	= m_playerHPclass->GetHP();		// �G��HP���擾
    int MAXHP	= m_playerHPclass->GetMaxHP();	// �ő�HP���擾

    // �`��ʒu�̃I�t�Z�b�g�l��΃Q�[�W�̕����v�Z����
    LONG offset = static_cast<LONG>(LEFT_POSITION);
    LONG width = static_cast<LONG>(offset + MAX_WIDTH * (m_playerHP / MAXHP));

	// �Q�[�W�͈̔͂̐ݒ�
    RECT outline{ offset - 2, TOP_POSITION - 2, offset + MAX_WIDTH + 2, BOTTOM_POSITION + 2 };
    RECT back{ offset, TOP_POSITION, offset + MAX_WIDTH, BOTTOM_POSITION };
    RECT gauge{ offset, TOP_POSITION, width, BOTTOM_POSITION };

    // �X�v���C�g�o�b�`���J�n����
    m_spriteBatch->Begin();

    // �Q�[�W�̕`��
    m_spriteBatch->Draw(m_texture.Get(), outline, DirectX::Colors::Black);
    m_spriteBatch->Draw(m_texture.Get(), back, DirectX::Colors::BlanchedAlmond);  // �w�ʂ̕`��
    m_spriteBatch->Draw(m_texture.Get(), gauge, DirectX::Colors::LimeGreen);            // �Q�[�W����

    // �X�v���C�g�o�b�`���I������
    m_spriteBatch->End();
}


// ----------------------------
/// <summary>a
/// �I���֐�
/// </summary>
// ----------------------------
void PlayerHPUI::Finalize()
{
	m_spriteBatch.reset();
	m_texture.Reset();
	m_pDR = nullptr;
	m_playerHPclass = nullptr;
}