// --------------------------------------------------
// 
// ���O:	ResultScene.h
// ���e:	���U���g�V�[����`�悷��N���X
// �쐬:	�r�c����
// 
// --------------------------------------------------
// �C���N���[�h
#include "pch.h"
#include "ResultScene.h"
#include "Game/CommonResources.h"
#include "Game/GameResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/Texture.h"
#include "../Sound/Sound.h"

// ---------------------------------------------
/// <summary>
/// �R���X�g���N�^
/// </summary>
// ---------------------------------------------
ResultScene::ResultScene()
	: m_spriteBatch{}
	, m_texture{}
	, m_texCenter{}
	, m_isChangeScene{}
{
	// �X�N���[���V���b�g���擾
	m_captureTexture = GameData::GetInstance()->GetScreenShot();
}

// ---------------------------------------------
/// <summary>
/// �f�X�g���N�^
/// </summary>
// ---------------------------------------------
ResultScene::~ResultScene()
{
}

// ---------------------------------------------
/// <summary>
/// ����������
/// </summary>
// ---------------------------------------------
void ResultScene::Initialize()
{
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// �X�v���C�g�o�b�`���쐬����
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	// �e�N�X�`���̍쐬����уf�[�^�̎擾
	CreateTextures();
	// �V�[���ύX�t���O������������
	m_isChangeScene = false;
}

// ---------------------------------------------
/// <summary>
/// �e�N�X�`�����쐬���A�f�[�^���擾����
/// </summary>
// ---------------------------------------------
void ResultScene::CreateTextures()
{
	// �f�o�C�X�̎擾
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	// �e�N�X�`���̍쐬
	m_texture = GameResources::GetInstance()->GetTexture("result");

	DirectX::SimpleMath::Vector2 texSize{};

	// �e�N�X�`���̃T�C�Y�ƒ��S�_���v�Z����
	mylib::Texture::CalculateTextureCenter
	(
		m_texture,
		texSize,
		m_texCenter
	);

	// �X�N���[���V���b�g�̃T�C�Y�ƒ��S�_���v�Z����
	mylib::Texture::CalculateTextureCenter
	(
		m_captureTexture,
		texSize,
		m_captureTexCenter
	);
}

// ---------------------------------------------
/// <summary>
/// �V�[�����X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
// ---------------------------------------------
void ResultScene::Update(float elapsedTime)
{
	// �L�[�{�[�h�X�e�[�g�g���b�J�[���擾����
	const auto& kbTracker = CommonResources::GetInstance()->GetInputManager()->GetKeyboardTracker();

	// �X�y�[�X�L�[�������ꂽ��
	if (kbTracker->pressed.Space)
	{
		m_isChangeScene = true;
	}
}

// ---------------------------------------------
/// <summary>
/// �`�悷��
/// </summary>
// ---------------------------------------------
void ResultScene::Render()
{
	// �R�����X�e�[�g���擾����
	auto states = CommonResources::GetInstance()->GetCommonStates();

	// �X�v���C�g�o�b�`�̊J�n�F�I�v�V�����Ń\�[�g���[�h�A�u�����h�X�e�[�g���w�肷��
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, states->NonPremultiplied());
	
	RECT rect{ CommonResources::GetInstance()->GetDeviceResources()->GetOutputSize() };
	// �X�N���[���̒��S���v�Z����
	DirectX::SimpleMath::Vector2 pos{ rect.right / 2.0f, rect.bottom / 2.0f };
	DirectX::SimpleMath::Vector2 pos2{ 350.0f , rect.bottom / 2.0f };


	m_spriteBatch->Draw(
		m_texture.Get(),			// �e�N�X�`��(SRV)
		pos,						// �X�N���[���̕\���ʒu(origin�̕`��ʒu)
		nullptr,					// ��`(RECT)
		DirectX::Colors::White,		// �w�i�F
		0.0f,// ��]�p(���W�A��)
		m_texCenter,				// �e�N�X�`���̊�ɂȂ�\���ʒu(�`�撆�S)(origin)
		1.0f,						// �X�P�[��(scale)
		DirectX::SpriteEffects_None,// �G�t�F�N�g(effects)
		0.0f						// ���C���[�x(�摜�̃\�[�g�ŕK�v)(layerDepth)
	);

	// �w�i�O�ʂɃQ�[���f�[�^�����X�N�V����`��
	if (GameData::GetInstance()->GetScreenShot())
	{
		m_spriteBatch->Draw(
			m_captureTexture.Get(),
			pos2,
			nullptr,
			DirectX::Colors::White,
			DirectX::XMConvertToRadians(-20.0f),// ��]�p(���W�A��)
			m_captureTexCenter,
			0.5f,
			DirectX::SpriteEffects_None,
			0.0f
		);
	}

	// �X�v���C�g�o�b�`�̏I���
	m_spriteBatch->End();
}

// ---------------------------------------------
/// <summary>
/// ��n������
/// </summary>
// ---------------------------------------------
void ResultScene::Finalize()
{
}

// ---------------------------------------------
/// <summary>
/// ���̃V�[��ID���擾����
/// </summary>
/// <returns>���̃V�[��ID</returns>
// ---------------------------------------------
IScene::SceneID ResultScene::GetNextSceneID() const
{
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
		return IScene::SceneID::TITLE;
	}

	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::NONE;
}