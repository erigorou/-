// --------------
//
// EnemyHPUI.cpp
//
// --------------

#include "pch.h"
#include "Game/UI/Header/EnemyHPUI.h"
#include "Game/Enemy/EnemyHP.h"
#include "Game/Screen.h"


// ----------------------------
/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="enemyHp">�G��HP����������N���X�̃|�C���^</param>
// ----------------------------
EnemyHPUI::EnemyHPUI(EnemyHP* enemyHp)
	: m_enemyHPclass(enemyHp)
	, m_spriteBatch()
	, m_enemyHP(0)
{
}

// ----------------------------
/// <summary>
/// �f�X�g���N�^
/// </summary>
// ----------------------------
EnemyHPUI::~EnemyHPUI()
{
}


void EnemyHPUI::Initialize(DX::DeviceResources* pDR)
{
	m_pDR = pDR;
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(pDR->GetD3DDeviceContext());	// �X�v���C�g�o�b�`�̐ݒ�
	
	LoadTexture(ENEMY_HP_TEXTURE);			// �e�N�X�`���̓ǂݍ���
	m_enemyHP = m_enemyHPclass->GetMaxHP();	// �G�l�~�[��HP���擾
}


/// <summary>
/// �e�N�X�`���̓ǂݍ��݊֐�
/// </summary>
/// <param name="path">�t�@�C���p�X</param>
void EnemyHPUI::LoadTexture(const wchar_t* path)
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
void EnemyHPUI::Update()
{
}


// ----------------------------
/// <summary>
/// �`��֐�
/// </summary>
// ----------------------------
void EnemyHPUI::Render()
{
    m_enemyHP	= m_enemyHPclass->GetHP();		// �G��HP���擾
    int MAXHP	= m_enemyHPclass->GetMaxHP();	// �ő�HP���擾

    // �`��ʒu�̃I�t�Z�b�g�l��΃Q�[�W�̕����v�Z����
    LONG offset = static_cast<LONG>(Screen::CENTER_X - (MAX_WIDTH / 2));
    LONG width	= static_cast<LONG>(offset + MAX_WIDTH * (m_enemyHP / MAXHP));

	// �Q�[�W�͈̔͂̐ݒ�
    RECT outline{ offset - 2, TOP_POSITION - 2, offset + MAX_WIDTH + 2, BOTTOM_POSITION + 2 };
    RECT back{ offset, TOP_POSITION, offset + MAX_WIDTH, BOTTOM_POSITION };
    RECT gauge{ offset, TOP_POSITION, width, BOTTOM_POSITION };

    // �X�v���C�g�o�b�`���J�n����
    m_spriteBatch->Begin();

    // �Q�[�W�̕`��
    m_spriteBatch->Draw(m_texture.Get(), outline, DirectX::Colors::Black);
    m_spriteBatch->Draw(m_texture.Get(), back, DirectX::Colors::BlanchedAlmond);  // �w�ʂ̕`��
    m_spriteBatch->Draw(m_texture.Get(), gauge, DirectX::Colors::Red);            // �Q�[�W����

    // �X�v���C�g�o�b�`���I������
    m_spriteBatch->End();
}


// ----------------------------
/// <summary>a
/// �I���֐�
/// </summary>
// ----------------------------
void EnemyHPUI::Finalize()
{
	m_spriteBatch.reset();
	m_texture.Reset();
	m_pDR = nullptr;
	m_enemyHPclass = nullptr;
}