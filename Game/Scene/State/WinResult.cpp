// ------------------------------------------------------------------------------
// ���O:	WinResult.cpp
// ���e:	�s�k���̃��U���g�V�[���N���X
// ����:	�r�c����
// ------------------------------------------------------------------------------
#include "pch.h"
#include "WinResult.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/Texture.h"
#include "Game/GameResources.h"

//---------------------------------------------------------
/// <summary>
/// �R���X�g���N�^
/// </summary>
// ---------------------------------------------------------
WinResult::WinResult()
	:
	m_spriteBatch{},
	m_texture{},
	m_texCenter{},
	m_isChangeScene{}
{
}

// ---------------------------------------------------------
/// <summary>
/// �f�X�g���N�^
/// </summary>
// ---------------------------------------------------------
WinResult::~WinResult()
{
	// do nothing.
}

// ---------------------------------------------------------
/// <summary>
/// �������������s��
/// </summary>
// ---------------------------------------------------------
void WinResult::Initialize()
{
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// �X�v���C�g�o�b�`���쐬����
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

	// �e�N�X�`���̍쐬����уf�[�^�̎擾
	m_texture = GameResources::GetInstance()->GetTexture("win");

	// �e�N�X�`���̑傫��
	DirectX::SimpleMath::Vector2 texSize;

	// �e�N�X�`���̒��S���W���v�Z����
	mylib::Texture::CalculateTextureCenter
	(
		m_texture,
		texSize,
		m_texCenter
	);

	// �V�[���ύX�t���O������������
	m_isChangeScene = false;
}

// ---------------------------------------------------------	
/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
// ---------------------------------------------------------
void WinResult::Update(float elapsedTime)
{
	// �錾���������A���ۂ͎g�p���Ă��Ȃ��ϐ�
	UNREFERENCED_PARAMETER(elapsedTime);
}

//---------------------------------------------------------
/// <summary>
/// �`�揈��
/// </summary>
// ---------------------------------------------------------
void WinResult::Render()
{
	auto states = CommonResources::GetInstance()->GetCommonStates();

	// �X�v���C�g�o�b�`�̊J�n�F�I�v�V�����Ń\�[�g���[�h�A�u�����h�X�e�[�g���w�肷��
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, states->NonPremultiplied());

	DirectX::SimpleMath::Vector2 texPos{ 0.0f, 200.0f };

	// �e�N�X�`����`�悷��
	mylib::Texture::DrawTexture
	(
		m_spriteBatch.get(),
		texPos,
		m_texture,
		m_texCenter,
		DirectX::SimpleMath::Vector2{ 1.0f, 1.0f },
		0.0f
	);

	// �X�v���C�g�o�b�`�̏I���
	m_spriteBatch->End();
}

//---------------------------------------------------------
/// <summary>
/// �I������
/// </summary>
// ---------------------------------------------------------
void WinResult::Finalize()
{
}

// ---------------------------------------------------------
/// <summary>
/// ���̃V�[��ID���擾����
/// </summary>
// ---------------------------------------------------------
IScene::SceneID WinResult::GetNextSceneID() const
{
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
		return IScene::SceneID::TITLE;
	}

	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::NONE;
}