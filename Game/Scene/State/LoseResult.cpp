// ------------------------------------------------------------------------------
// ���O:	LoseResult.cpp
// ���e:	�s�k���̃��U���g�V�[���N���X
// ����:	�r�c����
// ------------------------------------------------------------------------------

#include "pch.h"
#include "LoseResult.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/Texture.h"
#include "Game/GameResources.h"

const wchar_t* LoseResult::TEXTURE_PATH = L"Resources/Textures/UI/Lose.png";

// ---------------------------------------------
/// <summary>
/// �R���X�g���N�^
/// </summary>
// ---------------------------------------------
LoseResult::LoseResult()
	:
	m_spriteBatch{},
	m_texture{},
	m_texCenter{},
	m_isChangeScene{}
{
}

// ---------------------------------------------
/// <summary>
/// �f�X�g���N�^
/// </summary>
// ---------------------------------------------
LoseResult::~LoseResult()
{
	Finalize();
}

// ---------------------------------------------
/// <summary>
/// ����������
/// </summary>
// ---------------------------------------------
void LoseResult::Initialize()
{
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// �X�v���C�g�o�b�`���쐬����
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

	// �e�N�X�`�����擾
	m_texture = GameResources::GetInstance()->GetTexture("lose");

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

// ---------------------------------------------
/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
// ---------------------------------------------
void LoseResult::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}

// ---------------------------------------------
/// <summary>
/// �`�悷��
/// </summary>
// ---------------------------------------------

void LoseResult::Render()
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

// ---------------------------------------------
/// <summary>
/// ��n������
/// </summary>
// ---------------------------------------------
void LoseResult::Finalize()
{
}

// ---------------------------------------------
/// <summary>
/// ���̃V�[��ID���擾����
/// </summary>
/// <returns>���̃V�[��ID</returns>
// ---------------------------------------------
IScene::SceneID LoseResult::GetNextSceneID() const
{
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
		return IScene::SceneID::TITLE;
	}

	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::NONE;
}